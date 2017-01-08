
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0


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
    }
}

double randomVal(int max, int offset, double divisor) {
  double r = (double)(rand() % max - offset) / divisor;
  return r;
} void DVSetAllRandom(DVector* vector, int end) {
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

double feedForward(double total) {
    return calcTanh(total);
}


double train(Perceptron* perc, DVector* inputs, double answer) {
  if (DEBUG) {
    printf("----- ----- ----- -----\n");
  }
  double LR = 0.0006;
  double total = calcTotal(perc, inputs);
  double ff = feedForward(total);
  double grad = 2 * (answer - ff);
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
    printf("Answer: %lf ff: %lf Grad: %lf\n", answer, ff, grad);
    printf("Guessed/Actual: %lf/%lf\n", ff, answer);
  }
  int i;
  for(i = 0; i < perc->weights.size; i++) {
    perc->weights.data[i] += LR * grad * inputs->data[i];
  }
	printf("Diff (A/O/D): %lf/%lf/%lf\n", answer, ff, answer - ff);
  if (DEBUG) {
    printf("After weights: ");
    int p;
    for (p = 0; p < perc->weights.size; p++) {
      printf("%lf ", perc->weights.data[p]);
    }
    printf("\n");
  }
	return ff;
}

double average(double data[200]) {
	double total = 0;
	for (int i = 0; i < 200; i++) {
		total += data[i];
	}
	return total / 200;
}


double readTrainingDataFromConsole(DVector*, DVector*);
double readTrainingData(DVector*, DVector*);
void readAndTest(Perceptron*, double);
void readAndTestFromConsole(Perceptron*, double);
void runPerceptron() {
    Perceptron perc;
    initDVector(&perc.weights);
    DVSetAllRandom(&perc.weights, 1);
    DVector allInput;
    initDVector(&allInput);
    DVector answers;
    initDVector(&answers);
    //double scale = readTrainingData(&allInput, &answers);
    double scale = readTrainingData(&allInput, &answers);
		double diffs[200] = {0};
		int iout = 0;
    int i;
		for (int tries = 0; tries < 1000; tries++) {
			for (i = 0; i < allInput.size; i++) {
				DVector singleInput;
				initDVector(&singleInput);
				DVPush(&singleInput, allInput.data[i]);
				double answer = answers.data[i];
				double out = train(&perc, &singleInput, answer);
				diffs[iout] = out;
				iout++;
				printf("av: %lf\n", average(diffs));
				if(iout == 200) {
					iout = 0;
					if (average(diffs) < 0.002) {
						printf("Average is low\n");
						break;
					}
				}
			}
		}
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("Training done\n");
    }
    //readAndTest(&perc, scale);
    readAndTest(&perc, scale);
}

// END PERCEPTRON


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
    double x,a;
    char ch;
    sscanf(line, "%lf%c%lf", &x, &ch, &a);
    if (x == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, 1);
    DVPush(answers, a);
    if (fabs(x) > max) {
      max = fabs(x);
    }
	}
  fclose(filePtr);
  if (line) {
      free(line);
  }
  DVScale(input, max, 2);
	for(int i = 0; i < answers->size; i++) {
		answers->data[i] = answers->data[i] / max;
	}
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 2) {
      printf("%lf %lf\n", input->data[p], answers->data[p / 2]);
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
    double x;
    sscanf(line, "%lf", &x);
    DVPush(&singleInput, x);
    DVPush(&singleInput, 1);
    DVScale(&singleInput, scale, 2);
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("Input: %lf\n", singleInput.data[0]);
    }
    double ff = feedForward(calcTotal(perc, &singleInput));
		printf("%lf\n", ff * scale);
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
  while (1) {
    double x, a;
    char ch;
    scanf("%lf%c%lf", &x, &ch, &a);
    if (x == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, 1);
    DVPush(answers, a);
    if (fabs(x) > max) {
      max = fabs(x);
    }
	}
  DVScale(input, max, 2);
	for(int i = 0; i < answers->size; i++) {
		answers->data[i] = answers->data[i] / max;
	}
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 2) {
      printf("%lf %lf\n", input->data[p], answers->data[p / 2]);
    }
  }
  return max;
}

void readAndTestFromConsole(Perceptron* perc, double scale) {
  char str[20];
  while (scanf("%s", str) != EOF) {
    DVector singleInput;
    initDVector(&singleInput);
    double x;
    sscanf(str, "%lf", &x);
    DVPush(&singleInput, x);
    DVPush(&singleInput, 1);
    DVScale(&singleInput, scale, 2);
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("%lf\n", singleInput.data[0]);
    }
    double ff = feedForward(calcTotal(perc, &singleInput));
		printf("%lf\n", ff * scale);
  }
}


int main() {
  srand(time(0));
  runPerceptron();
  return (0);
}
