#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 1
#define NMAX 5


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
    for(i = 0; i < vector->size; i += numInputs) {
      if ((i + 1) % numInputs == 0) {
        i++;
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

double gradient(double answer, double total) {
  return (answer - output(total)) * calcDTanh(total);
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


double readTrainingDataFromConsole(DVector*, DVector*);
double readTrainingData(DVector*, DVector*);
void readAndTest(Perceptron*, double);
void readAndTestFromConsole(Perceptron*, double);
void runPerceptron() {
    int numInputs = 3;
    Perceptron perc;
    initDVector(&perc.weights);
    DVSetAllRandom(&perc.weights, numInputs);
    DVector allInput;
    initDVector(&allInput);
    DVector answers;
    initDVector(&answers);
    double scale = readTrainingData(&allInput, &answers);
    //double scale = readTrainingDataFromConsole(&allInput, &answers);
    int i;
    for (i = 0; i < allInput.size; i += numInputs) {
      DVector singleInput;
      initDVector(&singleInput);
      int j;
      for (j = 0; j < numInputs; j++) {
        DVPush(&singleInput, allInput.data[i + j]);
      }
      train(&perc, &singleInput, answers.data[i / numInputs]);
      if (DEBUG) {
        printf("%d ", i / numInputs);
      }
    }
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("Training done\n");
    }
    readAndTest(&perc, scale);
    //readAndTestFromConsole(&perc, scale);
}

// END PERCEPTRON

typedef struct {
    int layers;
    IVector widths;
    Perceptron percs[NMAX][NMAX];
} Network;

// Frist width value must include bias
initPVector(Network* network, int layers, int widths[NMAX]) {
  int i;
  network->layers = layers;
  initIVector(&network->widths);
  // Set width array to width input
  for (i = 0; i < layers; i++) {
      IVPush(&network->widths, widths[i] + 1);
  }
  // Set each input node weight to 1. Input nodes do not have weights
  //
  for (i = 0; i < widths[0] + 1; i++) {
    initDVector(&network->percs[0][i].weights);
    DVPush(&network->percs[0][i].weights, 1)
  }
  int layer;
  int neuron;
  for (layer = 0; layer < layers; layer++) {
    for (neuron = 0; neuron < widths[layer] + 1; neuron++) {
      initDVector(&network->percs[layer][neuron].weights);
      DVSetAllRandom(&network->percs[layer][neuron].weights, widths[layer - 1] + 1);
    }
  }
}

// Input including bias
double fwPropagate(Network* network, DVector* inputs) {
  int layer;
  int neu;
  int prev;
  double fwp;
  // Pass input through for input layer
  for (neu = 0; neu < network->widths[0]; neu++) {
    network->percs[0][neu].out = inputs[neu];
    network->percs[0][neu].total = inputs[neu];
  }
  // Set the output of each bias to 1;
  for (layer = 1; layer < network->layers; layer++) {
    network->percs[layer][network->widths[layer] - 1].out = 1;
    network->percs[layer][network->widths[layer] - 1].total = 1;
  }
  // Calculate forward propagation
  for(layer = 1; i < network->layers; layer++) {
    // For each neuron in layer
    for (neu = 0; neu < network->widths[layer] - 1; neu++) {
      // Adding each output from prev layer and calculating the tanh
      double total = 0;
      for (prev = 0; prev < network->widths[layer - 1]; prev++) {
        double weight = network->percs[layer][neu].weights[prev];
        double out = network->precs[layer - 1][prev].out;
        total += out * weight;
        network->precs[layer][neu].total += total;
        network->precs[layer][neu].out += output(total);
        fwp = network->precs[layer][neu].out;
      }
    }
  }
  return fwp;
}

// END NETWORK

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
      printf("%lf %lf %lf\n", input->data[p], input->data[p + 1], answers->data[p / 3]);
    }
  }
  return max;
}

// Reads coordinates from file and guesses output
void readAndTest(Perceptron* perc, double scale) {
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
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("%lf %lf\n", singleInput.data[0], singleInput.data[1]);
    }
    double ff = output(calcTotal(perc, &singleInput));
    if (ff > 0.0) {
      printf("+1\n");
    } else {
      printf("-1\n");
    }
    if (DEBUG && (sread = getline(&sline, &slength, sfilePtr)) != -1) {
      printf("%s\n", sline);
      double s;
      sscanf(sline, "%lf", &s);
      if ((s == 1.0 && ff > 0.0) || (s == -1.0 && ff <= 0.0)) {
        correct++;
      }
      totalCount++;
    }
  }
  if (DEBUG) {
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

void readAndTestFromConsole(Perceptron* perc, double scale) {
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
    double ff = output(calcTotal(perc, &singleInput));
    if (ff > 0.0) {
      printf("+1\n");
    } else {
      printf("-1\n");
    }
  }
}


int main() {
  srand(time(0));
  runPerceptron();
  return (0);
}
