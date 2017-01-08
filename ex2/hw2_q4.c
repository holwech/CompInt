
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
#define ANS 0
#define NMAX 10
#define AVG 100


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
    double x, a;
    char ch;
    sscanf(line, "%lf%c%lf", &x, &ch, &a);
    if (x == 0.0 && a == 0.0) {
      break;
    }
    DVPush(input, x);
    DVPush(answers, a);
    if (fabs(x) > max) {
      max = fabs(x);
    }
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
  DVScale(input, max);
	DVScale(answers, max);
  if (0) {
    int p;
    for (p = 0; p < input->size; p += 1) {
      printf("%lf a: %lf\n", input->d[p], answers->d[p]);
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


  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  sfilePtr = fopen("solution.txt", "r");
  if (sfilePtr == NULL) {
    printf("File open failed\n");
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
    double x;
    sscanf(line, "%lf", &x);
    if(0) {
      printf("%lf\n", x);
    }
    if (x == 0.0) {
      break;
    }
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
    DVector singleInput;
    initDVector(&singleInput);
    double x;
    sscanf(line, "%lf", &x);
    DVPush(&singleInput, x);
    DVScale(&singleInput, scale);
    if(0) {
      printf("----- ----- ----- -----\n");
      printf("%lf\n", singleInput.d[0]);
    }
    double ff = fwp(nw, &singleInput);
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
    DVPush(answers, a);
    if (fabs(x) > max) {
      max = fabs(x);
    }
  }
  DVScale(input, max);
	DVScale(answers, max);
  if (DEBUG) {
    int p;
    for (p = 0; p < input->size; p += 1) {
      printf("%lf %lf\n", input->d[p], answers->d[p]);
    }
  }
  return max;
}

void readAndTestFromConsole(Network* nw, double scale) {
  char str[20];
  while (scanf("%s", str) != EOF) {
    DVector singleInput;
    initDVector(&singleInput);
    double x;
    sscanf(str, "%lf", &x);
    DVPush(&singleInput, x);
    DVScale(&singleInput, scale);
    if(DEBUG) {
      printf("----- ----- ----- -----\n");
      printf("%lf\n", singleInput.d[0]);
    }
    double ff = fwp(nw, &singleInput);
		printf("%lf\n", ff * scale);
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

double train(Network* nw, DVector* inputs, double answer) {
	double out = fwp(nw, inputs);
	printf("Diff (A/O/D): %lf/%lf/%lf\n", answer, out, answer - out);
	bwp(nw, answer);
	return out;
}

double average(double diffs[AVG]) {
	double total = 0;
	for (int i = 0; i < AVG; i++) {
		total += fabs(diffs[i]);
	}
	return total / AVG;
}

int run(Network* nw, DVector* answers, DVector* inputs, double thres) {
	double diffs[AVG] = {0};
	int dcount = 0;
	int count = 1000;
	if (inputs->size < 20) {
		count = 100000;
	}
	for (int j = 0; j < count; j++) {
		for (int i = 0; i < answers->size; i += 1) {
			DVector input;
			initDVector(&input);
			DVPush(&input, inputs->d[i]);
			diffs[dcount] = answers->d[i] - train(nw, &input, answers->d[i]);
			dcount++;
			printf("av: %lf, thres: %lf\n", average(diffs), thres);
			if (dcount == AVG) {
				dcount = 0;
				if(average(diffs) < thres) {
					printf("av reached\n");
					return 1;
				}
			}
		}
	}
	return 0;
}

void resetNetwork(Network* nw) {
	for (int l = 0; l < nw->l - 1; l++) {
		nw->w[l]++;
		nw->p[l][nw->w[l] - 1].out = 1;
		nw->p[l][nw->w[l] - 1].total = 1;
	}
	// Set random weights
	for (int l = 1; l < nw->l; l++) {
		for (int n = 0; n < nw->w[l]; n++) {
			nw->p[l][n].w.size = 0;
			DVSetAllRandom(&nw->p[l][n].w, nw->w[l - 1]);
		}
	}
}

void classifyNLPoints() {
  DVector inputs;
  initDVector(&inputs);
  DVector answers;
  initDVector(&answers);
  double scale = readTrainingData(&inputs, &answers);
	Network nw;
	int widths[NMAX] = {1, 3, 3, 2, 1, 0, 0, 0, 0, 0};
	int layers = 5;
	double thres = 0.02;
	initNetwork(&nw, widths, layers);
	for(int tries = 0; tries < 10; tries++) {
		int done = run(&nw, &answers, &inputs, thres);
		if (done == 1) {
			readAndTest(&nw, scale);
			return;
		}
		resetNetwork(&nw);
	}
	readAndTest(&nw, scale);
}

int main() {
  srand(time(0));
  classifyNLPoints();
  return (0);
}

