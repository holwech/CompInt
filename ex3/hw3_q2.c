#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef enum { false, true } bool;
#define DEBUG 1
#define ANS 1
#define NMAX 10
#define MAX_POINTS 1000
#define N_X 20
#define N_Y 10

// END ARRAY

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

double randomDVal(double max, double offset, double divisor) {
	int newMax = max * 10000;
	int newOffset = offset * 10000;
	//printf("newMax: %d, newOffset: %d\n", newMax, newOffset);
  double r = (rand() % newMax - newOffset) / divisor;
	r = r / 10000;
	//printf("max: %lf, offset: %lf, rand: %lf\n", max, offset, r);
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
    char d[3000];
    int size;
} CVector;

void CVPrintAsChar(CVector* vector) {
  for (int row = 0; row < N_X * N_Y; row += N_X) {
    for (int col = 0; col < N_X; col++) {
      if (vector->d[row + col] == -1) {
        printf(". ");
      } else {
        printf("* ");
      }
    }
    printf("\n");
  }
  printf("\n\n");
}

void initCVector(CVector* vector) {
  vector->size = 0;
}

void CVPush(CVector* vector, char val) {
    vector->d[vector->size] = val;
    vector->size++;
}

// END CVECTOR


typedef struct {
  double w[N_X * N_Y];
  double x;
} Neuron;

typedef struct {
  Neuron neu[N_X * N_Y];
  CVector td[10];
  int numTDImg;
  CVector pd[10];
  int numPDImg;
} Hopfield;

void readFromFile(Hopfield*);
void initHopfield(Hopfield* hf) {
  // Set neuron output state
  for (int i = 0; i < 10; i++) {
    initCVector(&hf->td[i]);
    initCVector(&hf->pd[i]);
  }
  readFromFile(hf);
  // Calculate initial weights
  for (int image = 0; image < hf->numTDImg; image++) {
    for (int node = 0; node < N_X * N_Y; node++) {
      for (int weight = node; weight < N_X * N_Y; weight++) {
        if (node == weight) {
          hf->neu[node].w[weight] = 0;
          hf->neu[weight].w[node] = 0;
        } else {
          hf->neu[node].w[weight] += hf->td[image].d[weight];
          hf->neu[weight].w[node] += hf->td[image].d[weight];
        }
      }
    }
  }
}



void runHopfield() {
  Hopfield hf;
  initHopfield(&hf);

}

// END KMEANS


void readFromFile(Hopfield* hf) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  int image = 0;
  bool done = false;
  while (((read = getline(&line, &length, filePtr)) != -1) && !done) {
    for (int i = 0; i < 20; i++) {
      if (line[i] == '.') {
        CVPush(&hf->td[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->td[image], 1);
      } else if (line[0] == '-' && read == 2) {
        image++;
        printf("Training %d read\n", image);
        break;
      } else {
        image++;
        printf("Training done\n");
        done = true;
        break;
      }
    }
	}
  hf->numTDImg = image;
  done = false;
  image = 0;
  for (int i = 0; i < 20; i++) {
    if (line[i] == '.') {
      CVPush(&hf->pd[image], -1);
    } else if (line[i] == '*') {
      CVPush(&hf->pd[image], 1);
    } else if (line[0] == '-' && read == 2) {
      image++;
      printf("Problem %d read\n", image);
      break;
    }
  }
  while (((read = getline(&line, &length, filePtr)) != -1) && !done) {
    for (int i = 0; i < 20; i++) {
      if (line[i] == '.') {
        CVPush(&hf->pd[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->pd[image], 1);
      } else if (line[0] == '-' && read == 2) {
        image++;
        printf("Problem %d read\n", image);
        break;
      }
    }
  }
  image++;
  printf("PDIMG: %d\n", image);
  hf->numPDImg = image;
  if (DEBUG) {
    for (int i = 0; i < hf->numTDImg; i++) {
      CVPrintAsChar(&hf->td[i]);
    }
    for (int i = 0; i < hf->numPDImg; i++) {
      CVPrintAsChar(&hf->pd[i]);
    }
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
}


int main() {
  srand(time(0));
  runHopfield();
	return (0);
}
