#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 1
#define ANS 1
#define NMAX 10


typedef struct {
    double data[3000];
    int size;
} DVector;

void initDVector(DVector* vector) {
  vector->size = 0;
}

void DVPush(DVector* vector, double val) {
    vector->data[vector->size] = val;
    vector->size++;
}

void DVSetAll(DVector* vector, double val, int end) {
    int i;
    for(i = 0; i < end; i++) {
        vector->data[i] = val;
    }
}

void DVScale(DVector* vector, double scale, int numInputs) {
    int i;
    for(i = 0; i < vector->size; i++) {
      if ((i + 1) % numInputs == 0) {
        // Do no scale bias
      } else {
        vector->data[i] = vector->data[i] / scale;
      }
    }
}

double randomVal(int max, int offset, double divisor) {
  double r = (double)(rand() % max - offset) / divisor;
  return r;
}

void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        double r = randomVal(200, 100, 8500.0);
        //printf("Rval: %lf\n", r);
        DVPush(vector, r);
    }
}


// END DVECTOR

typedef struct {
  int data[3000];
    int size;
} IVector;

void initIVector(IVector* vector) {
  vector->size = 0;
}

void IVPush(IVector* vector, int val) {
    vector->data[vector->size] = val;
    vector->size++;
}

// END IVECTOR


typedef struct {
  DVector weights;
  double out;
  double total;
  double delta;
  int bias;
  double dw_ji;
} Perceptron;

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

double calcTanh(double tot) {
    if (0) {
      printf("Tanh: %lf, with input: %lf\n", tanh(tot), tot);
    }
    return tanh(tot);
}

double calcDTanh(double tot) {
    double DTanh = 1 - pow(tanh(tot), 2);
    if (0) {
      printf("DTanh: %lf, with input: %lf\n", DTanh, tot);
    }
    return DTanh;
}

int activate(double val) {
    if (val > 0) {
        return 1;
    } else {
        return -1;
    }
}

double calcTotal(Perceptron* perc, DVector* input) {
    double total = 0;
    int i;
    for (i = 0; i < perc->weights.size; i++) {
        total += input->data[i] * perc->weights.data[i];
    }
    if (DEBUG) {
      printf("Total: %lf \n", total);
    }
    return total;
}


double output(double total) {
    return calcTanh(total);
}

double calcGradient(double answer, double total) {
  return 2 * (answer - output(total)) * calcDTanh(total);
}


void train(Perceptron* perc, DVector* inputs, double answer) {
  if (DEBUG) {
    printf("----- ----- ----- -----\n");
  }
  double LR = 0.0006;
  double total = calcTotal(perc, inputs);
  double out = output(total);
  double grad = (answer - out) * calcDTanh(total);
  if (DEBUG) {
    printf("Input: ");
    int j;
    for (j = 0; j < inputs->size; j++) {
      printf("%lf ", inputs->data[j]);
    }
    printf("\n");
    printf("Before weights: ");
    int p;
    for (p = 0; p < perc->weights.size; p++) {
      printf("%lf, ", perc->weights.data[p]);
    }
    printf("\n");
    printf("Answer: %lf out: %lf Grad: %lf\n", answer, out, grad);
    printf("Guessed/Actual: %lf/%lf\n", out, answer);
  }
  int i;
  for(i = 0; i < perc->weights.size; i++) {
    perc->weights.data[i] += LR * grad * inputs->data[i];
  }
  if (DEBUG) {
    printf("After weights: ");
    int p;
    for (p = 0; p < perc->weights.size; p++) {
      printf("%lf ", perc->weights.data[p]);
    }
    printf("\n");
  }
}



// END PERCEPTRON

typedef struct {
    int layers;
    int widths[NMAX];
    Perceptron percs[NMAX][NMAX];
} Network;


void initNetwork(Network* nw, int layers, int widths[NMAX]) {
  int layer, neu;
  nw->layers = layers;
  for (layer = 0; layer < layers; layer++) {
    nw->widths[layer] = widths[layer] + 1;
    // If i is the last or first layer, remove bias
    if (layer == layers - 1 || layer == 0) {
      nw->widths[layer]--;
    }
  }
  //Set all weights in input to 1
  for (neu = 0; neu < nw->widths[0]; neu++) {
    initDVector(&nw->percs[0][neu].weights);
    DVPush(&nw->percs[0][neu].weights, 1);
    nw->percs[0][neu].bias = 0;
  }
  // Set the last node to be a bias
  nw->percs[0][nw->widths[0] - 1].bias = 1;
  // Set all weights in all other neurons to a small random value
  for (layer = 1; layer < nw->layers; layer++) {
    for (neu = 0; neu < nw->widths[layer]; neu++) {
      initDVector(&nw->percs[layer][neu].weights);
      DVSetAllRandom(&nw->percs[layer][neu].weights, nw->widths[layer - 1]);
      // Set the last neuron to be a bias, except on the output layer
      if (neu == nw->widths[layer] - 1 && layer != layers - 1) {
        nw->percs[layer][neu].bias = 1;
      } else {
        nw->percs[layer][neu].bias = 0;
      }
    }
  }
}

// Input including bias
double fwPropagate(Network* nw, DVector* input) {
  int layers = nw->layers;
  int layer, neu, w;
  // Propagate the input through the input layer
  for (neu = 0; neu < nw->widths[0]; neu++) {
    nw->percs[0][neu].out = input->data[neu];
    nw->percs[0][neu].total = input->data[neu];
  }
  // Set the output of all bias neurons
  for (layer = 0; layer < layers - 1; layer++) {
    nw->percs[layer][nw->widths[layer] - 1].out = 1;
    nw->percs[layer][nw->widths[layer] - 1].total = 1;
  }
  // Propagate through network
  for (layer = 1; layer < layers; layer++) {
    for (neu = 0; neu < nw->widths[layer]; neu++) {
      // If it is a bias node, don't do anything
      if (nw->percs[layer][neu].bias == 1) {
        continue;
      }
      double total;
      for (w = 0; w < nw->percs[layer][neu].weights.size; w++) {
        double weight = nw->percs[layer][neu].weights.data[w];
        double out = nw->percs[layer - 1][w].out;
        total += out * weight;
      }
      nw->percs[layer][neu].total = total;
      nw->percs[layer][neu].out = output(total);
    }
  }
  return nw->percs[layers - 1][0].out;
}



void bwPropagation(Network* nw, double answer) {
  int layer, neu, w;
  int LR = 0.0006;
  double newWeights[NMAX][NMAX][NMAX];
  int layers = nw->layers;
  // Calculate output layer delta
  // This is not generalized for multiple output neurons
  for (neu = 0; neu < nw->widths[layers - 1]; neu++) {
    if (nw->percs[layers - 1][neu].bias == 1) {
      continue;
    }
    double out = nw->percs[layers - 1][neu].out;
    double d_o = 2 * (answer - out);
    nw->percs[layers - 1][neu].delta = d_o;
    for (w = 0; w < nw->widths[layers - 2]; w++) {
      double oldWeight = nw->percs[layers - 1][neu].weights.data[w];
      newWeights[layers - 1][neu][w] = oldWeight + LR * d_o * out;
    }
  }
  // Propagate back through the rest of the network
  for (layer = layers - 2; layer > 0; layer--) {
    for (neu = 0; neu < nw->widths[layer]; neu++) {
      if (nw->percs[layer][neu].bias == 1) {
        continue;
      }
      int j;
      double dw_ji;
      for (j = 0; j < nw->widths[layer + 1]; j++) {
        double w_ji = nw->percs[layer + 1][j].weights.data[neu];
        double d_j = nw->percs[layer + 1][j].delta;
        dw_ji += d_j * w_ji;
      }
      nw->percs[layer][neu].dw_ji = dw_ji;
      nw->percs[layer][neu].delta = dw_ji * calcDTanh(nw->percs[layer][neu].total);
      for (w = 0; w < nw->widths[layer - 1]; w++) {
        double oldWeight = nw->percs[layer][neu].weights.data[w];
        double out_k = nw->percs[layer - 1][w].out;
        double d_i = nw->percs[layer][neu].delta;
        newWeights[layer][neu][w] = oldWeight + d_i * out_k;
      }
    }
  }
  for (layer = 1; layer < layers; layer++) {
    for (neu = 0; neu < nw->widths[layer]; neu++) {
      for (w = 0; w < nw->widths[layer]; w++) {
        nw->percs[layer][neu].weights.data[w] = newWeights[layer][neu][w];
      }
    }
  }
}


void printNetwork(Network nw) {
  int layer, neu, w;
  printf("----- NETWORK -----\n");
  printf("Layers: %d\n", nw.layers);
  printf("Widths: ");
  for (layer = 0; layer < nw.layers; layer++) {
    printf("%d, ", nw.widths[layer]);
  }
  printf("\n");
  for (layer = 0; layer < nw.layers; layer++) {
    for (neu = 0; neu < nw.widths[layer]; neu++) {
      printf(" + N: %d\n", neu);
      double total = nw.percs[layer][neu].total;
      double out = nw.percs[layer][neu].out;
      double delta = nw.percs[layer][neu].delta;
      double dw_ji = nw.percs[layer][neu].dw_ji;
      printf("Total: %lf, Out: %lf, Delta: %lf, dw_ji: %lf\n", total, out, delta, dw_ji);
      if (layer > 0) {
        printf("Input:  ");
        int i;
        for (i = 0; i < nw.widths[layer - 1]; i++) {
          printf("%lf, ", nw.percs[layer - 1][i].out);
        }
        printf("\n");
      }
      printf("Weights: ");
      for (w = 0; w < nw.widths[layer - 1]; w++) {
        printf("%lf, ", nw.percs[layer][neu].weights.data[w]);
      }
      printf("\n\n");
    }
    printf("-----\n");
  }
  printf("----- END -----\n\n");
}

void trainNetwork(Network* nw, DVector* inputs, double answer) {
  if (DEBUG) {
    printf("================================\n\n");
  }
  double guessed = fwPropagate(nw, inputs);
  if (1) {
    printNetwork(*nw);
    printf("Guessed/Answer: %lf/%lf\n", guessed, answer);
  }
  bwPropagation(nw, answer);
}

// END NETWORK

double readTrainingDataFromConsole(DVector*, DVector*);
double readTrainingData(DVector*, DVector*);
void readAndTest(Network*, double);
void readAndTestFromConsole(Network*, double);
void classifyNLPoints() {
  int width[NMAX] = {3, 3, 1, 0, 0, 0, 0, 0, 0, 0};
  int layers = 3;
  Network nw;
  initNetwork(&nw, layers, width);
  DVector inputs;
  initDVector(&inputs);
  DVector answers;
  initDVector(&answers);
  double scale = readTrainingData(&inputs, &answers);
  int j;
  for (j = 0; j < 1; j++) {
    int i;
    for (i = 0; i < answers.size; i += 3) {
      DVector input;
      initDVector(&input);
      DVPush(&input, inputs.data[i]);
      DVPush(&input, inputs.data[i + 1]);
      DVPush(&input, inputs.data[i + 2]);
      trainNetwork(&nw, &input, answers.data[i / 3]);
    }
  }
  readAndTest(&nw, scale);
}

// Reads training data
double readTrainingData(DVector* input, DVector* answers) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  double max = 0;
  while ((read = getline(&line, &length, filePtr)) != -1) {
    double x, y, a;
    char ch;
    sscanf(line, "%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
    if (x == 0.0 && y == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, y);
    DVPush(input, 1);
    DVPush(answers, a);
    if (fabs(x) > max && fabs(x) > fabs(y)) {
      max = fabs(x);
    } else if (fabs(y) > max) {
      max = fabs(y);
    }
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
  DVScale(input, max, 3);
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 3) {
      printf("%lf %lf %lf a: %lf\n", input->data[p], input->data[p + 1], input->data[p + 2], answers->data[p / 3]);
    }
  }
  return max;
}

// Reads coordinates from file and guesses output
void readAndTest(Network* nw, double scale) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  FILE* sfilePtr;
  size_t slength = 0;
  char* sline = 0;
  int sread;

  int correct = 0;
  int totalCount = 0;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  sfilePtr = fopen("solution.txt", "r");
  if (sfilePtr == NULL) {
    printf("File open failed\n");
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
    double x, y;
    char ch;
    sscanf(line, "%lf%c%lf", &x, &ch, &y);
    if(0) {
      printf("%lf %lf\n", x, y);
    }
    if (x == 0.0 && y == 0.0) {
      break;
    }
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
    DVector singleInput;
    initDVector(&singleInput);
    double x, y;
    char ch;
    sscanf(line, "%lf%c%lf", &x, &ch, &y);
    DVPush(&singleInput, x);
    DVPush(&singleInput, y);
    DVPush(&singleInput, 1);
    DVScale(&singleInput, scale, 3);
    if(DEBUG || ANS) {
      printf("----- ----- ----- -----\n");
      printf("%lf %lf\n", singleInput.data[0], singleInput.data[1]);
    }
    double ff = fwPropagate(nw, &singleInput);
    if (ff > 0.0) {
      printf("+1\n");
    } else {
      printf("-1\n");
    }
    if ((DEBUG || ANS) && (sread = getline(&sline, &slength, sfilePtr)) != -1) {
      printf("%s\n", sline);
      double s;
      sscanf(sline, "%lf", &s);
      if ((s == 1.0 && ff > 0.0) || (s == -1.0 && ff <= 0.0)) {
        correct++;
      }
      totalCount++;
    }
  }
  if (DEBUG || ANS) {
    printf("%d/%d\n", correct, totalCount);
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
  fclose(sfilePtr);
  if (sline) {
      free(sline);
  }
}

double readTrainingDataFromConsole(DVector* input, DVector* answers) {
  double max = 0;
  double min = 0;
  while (1) {
    double x, y, a;
    char ch;
    scanf("%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
    if (x == 0.0 && y == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, y);
    DVPush(input, 1);
    DVPush(answers, a);
    if (fabs(x) > max && fabs(x) > fabs(y)) {
      max = fabs(x);
    } else if (fabs(y) > max) {
      max = fabs(y);
    }
  }
  DVScale(input, max, 3);
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 3) {
      printf("%lf %lf %lf\n", input->data[p], input->data[p + 1], answers->data[p / 3]);
    }
  }
  return max;
}

void readAndTestFromConsole(Network* nw, double scale) {
  char str[20];
  while (scanf("%s", str) != EOF) {
    DVector singleInput;
    initDVector(&singleInput);
    double x, y;
    char ch;
    sscanf(str, "%lf%c%lf", &x, &ch, &y);
    DVPush(&singleInput, x);
    DVPush(&singleInput, y);
    DVPush(&singleInput, 1);
    DVScale(&singleInput, scale, 3);
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("%lf %lf\n", singleInput.data[0], singleInput.data[1]);
    }
    double ff = fwPropagate(nw, &singleInput);
    if (ff > 0.0) {
      printf("+1\n");
    } else {
      printf("-1\n");
    }
  }
}


int main() {
  srand(time(0));
  classifyNLPoints();
  return (0);
}


void getPercInput(Network* nw, DVector* input, int layer) {
  int i;
  for (i = 0; i < nw->widths[layer - 1]; i++) {
    DVPush(input, nw->percs[layer - 1][i].out);
  }
}

double fwPropagateOld(Network* nw, DVector* inputs) {
  int layer, neu, prev;
  // Pass input through for input layer
  for (neu = 0; neu < nw->widths[0]; neu++) {
    nw->percs[0][neu].out = inputs->data[neu];
    nw->percs[0][neu].total = inputs->data[neu];
  }
  // Set the output of each bias to 1
  for (layer = 1; layer < nw->layers - 1; layer++) {
    nw->percs[layer][nw->widths[layer] - 1].out = 1;
    nw->percs[layer][nw->widths[layer] - 1].total = 1;
  }
  // Calculate forward propagation
  for(layer = 1; layer < nw->layers; layer++) {
    // For each neuron in layer
    int offset = 0;
    // For last layer, exclude bias node
    if (layer < nw->layers - 1) {
      offset = -1;
    }
    for (neu = 0; neu < nw->widths[layer] + offset; neu++) {
      // Adding each output from prev layer and calculating the tanh
      double total = 0;
      for (prev = 0; prev < nw->widths[layer - 1]; prev++) {
        double weight = nw->percs[layer][neu].weights.data[prev];
        double out = nw->percs[layer - 1][prev].out;
        total += out * weight;
      }
      nw->percs[layer][neu].total = total;
      if (0) { // DEBUG START
        printf("Total: %lf, Out: %lf\n", total, output(total));
      } // DEBUG END
      if(layer == nw->layers - 1) {
        nw->percs[layer][neu].out = output(total);
      } else {
        nw->percs[layer][neu].out = output(total);
      }
    }
  }
  return nw->percs[nw->layers - 1][0].out;
}


// Frist width value must include bias
initNetworkOld(Network* nw, int layers, int widths[NMAX]) {
  int i;
  nw->layers = layers;
  // Set width array to width input plus an addition bias
  for (i = 0; i < layers; i++) {
    if (i < layers - 1 && i != 0) { // First layer already has bias included
      nw->widths[i] = widths[i] + 1;
    } else { // Output layer has not bias neuron
      nw->widths[i] = widths[i];
    }
  }
  // Set each input node weight to 1, since input nodes do not have weights
  for (i = 0; i < nw->widths[0]; i++) {
    initDVector(&nw->percs[0][i].weights);
    DVPush(&nw->percs[0][i].weights, 1);
  }
  int layer;
  int neuron;
  // Set all other weights to a small random value
  for (layer = 1; layer < layers; layer++) {
    for (neuron = 0; neuron < nw->widths[layer]; neuron++) {
      initDVector(&nw->percs[layer][neuron].weights);
      DVSetAllRandom(&nw->percs[layer][neuron].weights, nw->widths[layer - 1]);
    }
  }
}



void bwPropagationOld(Network* nw, double answer) {
  int neu, w, layer, out_ji;
  int layers = nw->layers;
  double LR = 0.009;
  double newWeights[NMAX][NMAX][NMAX];
  int printCount = 0;
  if (DEBUG) { // DEBUG START
    printf("----- ----- ----- -----\n");
    printf("Layer: %d\n", layers - 1);
  } // DEBUG END
  // Calculates the delta for each node of the output layer
  for (neu = 0; neu < nw->widths[layers - 1]; neu++) {
    double net_o = nw->percs[layers - 1][neu].total;
    double out = nw->percs[layers - 1][neu].out;
    double delta = 2 * (answer - out);
    nw->percs[nw->layers - 1][neu].delta = delta;
    if (DEBUG) { // DEBUG START
      printf("----- neuron %d -----\n", neu);
      printf("net_o: %lf, delta: %lf, error: %lf\n", net_o, delta, answer - out);
      printCount++;
    }
    // For each weight of a node calculate new weight
    for (w = 0; w < nw->widths[layers - 2]; w++) {
      double prevWeight = nw->percs[layers - 1][neu].weights.data[w];
      double out_ji = nw->percs[layers - 2][w].out;
      newWeights[layers - 1][neu][w] = prevWeight + LR * delta * out_ji;
      if (DEBUG) { // DEBUG START
        printf("Old weight: %lf, New weight: %lf Delta weight: %lf\n",
                prevWeight,
                newWeights[layers - 1][neu][w],
                LR * delta * out_ji);
      } // DEBUG END
    }
  }
  printCount = 0;
  // Calculate new weights for the rest of the nw excluding input layer
  for (layer = layers - 2; layer > 0; layer--) {
    if(DEBUG) { // DEBUG START
      printf("Layer: %d\n", layer);
    } // DEBUG END
    for (neu = 0; neu < nw->widths[layer] - 1; neu++) {
      double net_i = nw->percs[layer][neu].total;
      double dw_ji = 0;
      int out_ji;
      // If the next layer is not the output layer, bias should be skipped
      int offset = -1;
      if (layer == layers - 2) {
        offset = 0;
      }
      // Calculate the sum of all delta * weight values going from i to j
      for (out_ji = 0; out_ji < nw->widths[layer + 1] + offset; out_ji++) {
          double delta_j = nw->percs[layer + 1][out_ji].delta;
          double w_ji = nw->percs[layer + 1][out_ji].weights.data[neu];
          dw_ji +=  delta_j * w_ji;
          if (DEBUG && printCount == neu) { // DEBUG START
            printf("----- neuron %d -----\n", neu);
            printf("net_i: %lf, dw_ji: %lf, delta: ", net_i, dw_ji);
          } // DEBUG END
      }
      double delta = calcDTanh(nw->percs[layer][neu].out) * dw_ji;
      if (DEBUG && printCount == neu) {
        printf("%lf\n");
        printCount++;
      }
      // For each weight of a node calculate new weight
      for (w = 0; w < nw->widths[layer - 1]; w++) {
        double prevWeight = nw->percs[layer][neu].weights.data[w];
        double out_k = nw->percs[layer - 1][w].out;
        newWeights[layer][neu][w] = prevWeight + LR * delta * out_k;
        if (DEBUG) { // DEBUG START
          printf("Old weight: %lf, New weight: %lf Delta weight: %lf\n",
                  prevWeight,
                  newWeights[layer][neu][w],
                  LR * delta * out_k);
        } // DEBUG END
      }
      if (DEBUG) { printf("\n"); }
    }
  }
  // update weights
  int printL = layers - 2;
  int printN = 0;
  for (layer = layers - 2; layer > 0; layer--) {
    for (neu = 0; neu < nw->widths[layer] - 1; neu++) {
      for (w = 0; w < nw->widths[layer - 1]; w++) {
        if (0) { // DEBUG START
          if (printL == layer) {
            printf("----- ----- ----- -----\n");
            printf("Layer: %d\n", printL);
            printL++;
          }
          if (printN == neu) {
            printf("----- neuron %d -----\n", neu);
            printN++;
          }
          printf("Old weight: %lf, New weight: %lf\n",
                  nw->percs[layer][neu].weights.data[w],
                  newWeights[layer][neu][w]);
        } // DEBUG END
        nw->percs[layer][neu].weights.data[w] = newWeights[layer][neu][w];
      }
    }
  }
}


void printNetworkOld(Network* nw) {
  printf("----- NETWORK -----\n");
  printf("Layers: %d, ", nw->layers);
  printf("Widths: ");
  int i;
  for (i = 0; i < NMAX; i++) {
    printf("%d, ", nw->widths[i]);
  }
  printf("\n");
  int layer, neu, w;
  for (layer = 0; layer < nw->layers; layer++) {
    printf("----- L: %d -----\n", layer);
    for (neu = 0; neu < nw->widths[layer]; neu++) {
      printf(" + N: %d\n", neu);
      double total = nw->percs[layer][neu].total;
      double out = nw->percs[layer][neu].out;
      double delta = nw->percs[layer][neu].delta;
      printf("Total: %lf, Out: %lf, Delta: %lf\n", total, out, delta);
      if (layer > 0)  {
        printf("Input: ");
        for (i = 0; i < nw->percs[layer][neu].weights.size; i++) {
          printf("%lf, ", nw->percs[layer - 1][i].out);
        }
        printf("\n");
      }
      printf("Weights: ");
      for (w = 0; w < nw->percs[layer][neu].weights.size; w++) {
        printf("%lf, ", nw->percs[layer][neu].weights.data[w]);
      }
      printf("\n");
    }
  }
  printf("----- END -----\n\n");
}
