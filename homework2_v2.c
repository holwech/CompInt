#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 1


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
      vector->data[i] = vector->data[i] / scale;
      vector->data[i + 1] = vector->data[i + 1] / scale;
    }
}

double randomVal(int max, int offset, double divisor) {
  double r = (double)(rand() % max - offset) / divisor;
  return r;
} void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        double r = randomVal(100, 50, 100000.0);
        //printf("Rval: %lf\n", r);
        DVPush(vector, r);
    }
}


// END DVECTOR

typedef struct {
  int data[3000];
    int size;
} IVector;

void IVPush(IVector* vector, int val) {
    vector->data[vector->size] = val;
    vector->size++;
}

// END IVECTOR
typedef struct {
  DVector weights;
} Perceptron;

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

double calcTanh(double tot) {
    return tanh(tot);
}

int activate(double val) {
    if (val > 0) {
        return 1;
    } else {
        return -1;
    }
}

double feedForward(Perceptron* perc, DVector* input) {
    double total = 0;
    int i;
    for (i = 0; i < perc->weights.size - 1; i++) {
        total += input->data[i] * perc->weights.data[i];
    }
    total = calcTanh(total);
    if (DEBUG) {
      printf("FF Tot/Sig: %lf \n", total);
    }
    if (0) {
      printf("%lf\n", total);
    }
    return total;
}

void train(Perceptron* perc, DVector* inputs, double answer) {
  if (DEBUG) {
    printf("----- ----- ----- -----\n");
  }
  double LR = 0.01;
  double ff = feedForward(perc, inputs);
  double error = answer - ff;
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
    printf("Answer: %lf ff: %lf Error: %lf\n", answer, ff, error);
    printf("Guessed/Actual: %lf/%lf\n", ff, answer);
  }
  int i;
  for(i = 0; i < perc->weights.size; i++) {
    perc->weights.data[i] += LR * error * inputs->data[i];
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


void readAndTest(Perceptron*, double);
double readTrainingData(DVector*, DVector*);
void testPercFromFile(Perceptron*);
void readData(DVector*);
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
    int i;
    for (i = 0; i + numInputs < allInput.size; i += numInputs) {
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
}

// END PERCEPTRON


// Reads training data
double readTrainingData(DVector* input, DVector* answers) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  ssize_t read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  double max = 0;
  double min = 0;
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
  ssize_t read;

  FILE* sfilePtr;
  size_t slength = 0;
  char* sline = 0;
  ssize_t sread;

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
    double ff = feedForward(perc, &singleInput);
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

// Reads data from console input
// OUTDATED
void readData(DVector* input) {
  char str[100];
  int i;
  while(1)  {
    double x, y, a;
    char ch;
    scanf("%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
    if(DEBUG) {
      printf("%lf %lf %lf\n", x, y, a);
    }
    if (x == 0.0 && y == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, y);
    DVPush(input, a);
  }
}

// Guesses output from console input
void testPerc(Perceptron* perc, double scale) {
  while(1)  {
    DVector singleInput;
    double x, y;
    char ch;
    scanf("%lf%c%lf", &x, &ch, &y);
    if(DEBUG) {
      printf("%lf %lf\n", x, y);
    }
    DVPush(&singleInput, x);
    DVPush(&singleInput, y);
    DVPush(&singleInput, 1);
    double ff = feedForward(perc, &singleInput);
    if (ff > 0) {
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
