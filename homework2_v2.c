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

double randomVal(int max, int offset, double divisor) {
  double r = (double)(rand() % max - offset) / divisor;
  return r;
}

void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        double r = randomVal(100, 50, 10000.0);
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

int activate(double val) {
    if (val > 0) {
        return 1;
    } else {
        return 0;
    }
}

double feedForward(Perceptron* perc, DVector* input) {
    double total = 0;
    int i;
    for (i = 0; i < perc->weights.size; i++) {
        total += input->data[i] * perc->weights.data[i];
    }
    if (DEBUG) {
      printf("FF Tot/Sig: %lf \n", total);
    }
    if (0) {
      printf("%lf\n", total);
    }
    return activate(total);
}

void train(Perceptron* perc, DVector* inputs, double answer) {
  if (DEBUG) {
    printf("----- ----- ----- -----\n");
  }
  double LR = 0.001;
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


void readAndTest(Perceptron*);
void readTrainingData(DVector*);
void testPercFromFile(Perceptron*);
void readData(DVector*);
void runPerceptron() {
    Perceptron perc;
    initDVector(&perc.weights);
    DVSetAllRandom(&perc.weights, 3);
    DVector allInput;
    initDVector(&allInput);
    readTrainingData(&allInput);
    int i;
    for (i = 0; i + 2 < allInput.size; i += 3) {
      DVector singleInput;
      initDVector(&singleInput);
      DVPush(&singleInput, allInput.data[i]);
      DVPush(&singleInput, allInput.data[i + 1]);
      DVPush(&singleInput, 1);
      double answer = allInput.data[i + 2];
      //printf("SingleInput: %lf %lf %lf\n", singleInput.data[0],singleInput.data[1],singleInput.data[2]);
      train(&perc, &singleInput, answer);
    }
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("Training done\n");
    }
    readAndTest(&perc);
}

// END PERCEPTRON

// Reads data from console input
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

// Reads training data
void readTrainingData(DVector* input) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  ssize_t read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
    double x, y, a;
    char ch;
    sscanf(line, "%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
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
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

// Reads coordinates from file and guesses output
void readAndTest(Perceptron* perc) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  ssize_t read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
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
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

// Guesses output from console input
void testPerc(Perceptron* perc) {
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
