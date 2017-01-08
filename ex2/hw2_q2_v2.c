#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
#define ANS 1
#define NMAX 10


typedef struct {
    double d[3000];
    int size;
} DVector;

void initDVector(DVector* vector) {
  vector->size = 0;
}

void DVPush(DVector* vector, double val) {
    vector->d[vector->size] = val;
    vector->size++;
}

void DVSetAll(DVector* vector, double val, int end) {
    int i;
    for(i = 0; i < end; i++) {
        vector->d[i] = val;
    }
}

void DVScale(DVector* vector, double scale) {
    int i;
    for(i = 0; i < vector->size; i++) {
			vector->d[i] = vector->d[i] / scale;
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
  DVector w;
  double out;
  double total;
  double delta;
	int bias;
} Perceptron;

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



// END PERCEPTRON

typedef struct {
    int l;
    int w[NMAX];
    Perceptron p[NMAX][NMAX];
} Network;


double fwp(Network*, DVector*);


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
  DVScale(input, max);
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 2) {
      printf("%lf %lf a: %lf\n", input->d[p], input->d[p + 1], answers->d[p / 2]);
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
    DVScale(&singleInput, scale);
    if(DEBUG || ANS) {
      printf("----- ----- ----- -----\n");
      printf("%lf %lf\n", singleInput.d[0], singleInput.d[1]);
    }
    double ff = fwp(nw, &singleInput);
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
			printf("FF: %lf\n", ff);
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
  while (1) {
    double x, y, a;
    char ch;
    scanf("%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
    if (x == 0.0 && y == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(input, y);
    DVPush(answers, a);
    if (fabs(x) > max && fabs(x) > fabs(y)) {
      max = fabs(x);
    } else if (fabs(y) > max) {
      max = fabs(y);
    }
  }
  DVScale(input, max);
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 2) {
      printf("%lf %lf %lf\n", input->d[p], input->d[p + 1], answers->d[p / 3]);
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
    DVScale(&singleInput, scale);
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("%lf %lf\n", singleInput.d[0], singleInput.d[1]);
    }
    double ff = fwp(nw, &singleInput);
    if (ff > 0.0) {
      printf("+1\n");
    } else {
      printf("-1\n");
    }
  }
}

void initNetwork(Network* nw, int widths[NMAX], int layers) {
	nw->l = layers;
	for (int l = 0; l < NMAX; l++) {
		// Copy width
		nw->w[l] = widths[l];
	}
	nw->p[nw->l - 1][0].bias = 0;
	for (int l = 0; l < nw->l - 1; l++) {
		nw->w[l]++;
		nw->p[l][nw->w[l] - 1].out = 1;
		nw->p[l][nw->w[l] - 1].total = 1;
		nw->p[l][nw->w[l] - 1].bias = 1;
		for (int n = 0; n < nw->w[l] - 1; n++) {
			nw->p[l][n].bias = 0;	
		}
	}
	// Set random weights
	for (int l = 1; l < nw->l; l++) {
		for (int n = 0; n < nw->w[l]; n++) {
			DVSetAllRandom(&nw->p[l][n].w, nw->w[l - 1]);
		}
	}
}

double fwp(Network* nw, DVector* input) {
	// Set input to output on input layer
	for (int n = 0; n < input->size; n++) {
		if(nw->p[0][n].bias == 1) {
			continue;
		}
		nw->p[0][n].out = input->d[n];
		nw->p[0][n].total = input->d[n];
	}
	// Propagate
	for (int l = 1; l < nw->l; l++) {
		for (int n = 0; n < nw->w[l]; n++) {
			if (nw->p[l][n].bias == 1) {
				continue;
			}	
			double total = 0;
			for (int w = 0; w < nw->w[l - 1]; w++) {
				double weight = nw->p[l][n].w.d[w];
				double out_k = nw->p[l - 1][w].out;
				total += weight * out_k;	
			}
			nw->p[l][n].total = total;
			nw->p[l][n].out = calcTanh(total);
		}
	}
	return nw->p[nw->l - 1][0].out;
}

void bwp(Network* nw, double answer) {
	double LR = 0.0001;
	double newWeights[NMAX][NMAX][NMAX];
	for (int l = 0; l < nw->l; l++) {
		for (int n = 0; n < nw->w[l]; n++) {
			for (int w = 0; w < nw->w[l - 1]; w++) {
				newWeights[l][n][w] = 0;			
			}
		}
	}
	// Calculate for output layer
	double out = nw->p[nw->l - 1][0].out;
	nw->p[nw->l - 1][0].delta = 2 * (answer - out);
	// Calculate new weights for input layer
	for (int w = 0; w < nw->w[nw->l - 2]; w++) {
		double oldWeight = nw->p[nw->l - 1][0].w.d[w];
		double delta = nw->p[nw->l - 1][0].delta; 
		newWeights[nw->l - 1][0][w] = oldWeight + LR * delta * out;
	}
	for(int l = nw->l - 2; l > 0; l--) {
		for(int n = 0; n < nw->w[l]; n++) {
			if (nw->p[l][n].bias == 1) {
				continue;
			}
			double dw_ji = 0;
			for (int j = 0; j < nw->w[l + 1]; j++) {
				if (nw->p[l + 1][j].bias == 1) {
					continue;
				}
				double w_ji = nw->p[l + 1][j].w.d[n];
				double delta_j = nw->p[l + 1][j].delta;
				dw_ji += delta_j * w_ji;
			}
			nw->p[l][n].delta = dw_ji * calcDTanh(nw->p[l][n].total);
			double delta = nw->p[l][n].delta;
			for (int k = 0; k < nw->w[l - 1]; k++) {
				double oldWeight = nw->p[l][n].w.d[k];
				double out_k = nw->p[l - 1][k].out;
				newWeights[l][n][k] = oldWeight + LR * delta * out_k;
			}
		}
	}
	for (int l = 0; l < nw->l; l++) {
		for (int n = 0; n < nw->w[l]; n++) {
			for (int w = 0; w < nw->w[l - 1]; w++) {
				nw->p[l][n].w.d[w] = newWeights[l][n][w];	
			}
		}
	}
}

void printNetwork(Network* nw) {
  printf("----- NETWORK -----\n");
  printf("Layers: %d\n", nw->l);
  printf("Widths: ");
  for (int layer = 0; layer < nw->l; layer++) {
    printf("%d, ", nw->w[layer]);
  }
  printf("\n");
  for (int layer = 0; layer < nw->l; layer++) {
    for (int neu = 0; neu < nw->w[layer]; neu++) {
      printf(" + N: %d\n", neu);
      double total = nw->p[layer][neu].total;
      double out = nw->p[layer][neu].out;
      double delta = nw->p[layer][neu].delta;
      printf("Total: %lf, Out: %lf, Delta: %lf\n", total, out, delta);
      if (layer > 0) {
        printf("Input:  ");
        int i;
        for (i = 0; i < nw->w[layer - 1]; i++) {
          printf("%lf, ", nw->p[layer - 1][i].out);
        }
        printf("\n");
      }
      printf("Weights: ");
      for (int w = 0; w < nw->w[layer - 1]; w++) {
        printf("%lf, ", nw->p[layer][neu].w.d[w]);
      }
      printf("\n\n");
    }
    printf("-----\n");
  }
  printf("----- END -----\n\n");
}

void train(Network* nw, DVector* inputs, double answer) {
	fwp(nw, inputs);
	bwp(nw, answer);
}

void classifyNLPoints() {
	Network nw;
	int widths[NMAX] = {2, 3, 1, 0, 0, 0, 0, 0, 0, 0};
	int layers = 3;
	initNetwork(&nw, widths, layers);

  DVector inputs;
  initDVector(&inputs);
  DVector answers;
  initDVector(&answers);
  double scale = readTrainingDataFromConsole(&inputs, &answers);
	for (int j = 0; j < 10000; j++) {
		for (int i = 0; i < answers.size; i += 2) {
      DVector input;
      initDVector(&input);
      DVPush(&input, inputs.d[i]);
      DVPush(&input, inputs.d[i + 1]);
      train(&nw, &input, answers.d[i / 2]);
		}
	}
	readAndTestFromConsole(&nw, scale);
}

int main() {
  srand(time(0));
  classifyNLPoints();
  return (0);
}

