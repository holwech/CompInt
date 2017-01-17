#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
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
  for (int row = 0; row < N_X * N_Y; row += N_Y) {
    for (int col = 0; col < N_Y; col++) {
      printf("%c ", vector.d[row + col]);
    }
    printf("\n");
  }
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
  CVector td;
  int numTDImg;
  CVector pd;
  int numPDImg;
} Hopfield;

void initHopfield(Hopfield* hf) {
  // Set neuron output state
  for (int i = 0; i < 10; i++) {
    initCVector(&hf.td[i]);
    initCVector(&hf.pd[i]);
  }
  readFromFile(hf);
  // Calculate initial weights
  for (int image = 0; image < hf->numTDImg; image++) {
    for (int pixel = 0; pixel < N_X * N_Y; pixel++) {
      for (int weight = pixel; weight < N_X * N_Y; weight++) {
        if (pixel == weight) {
          hf->neu[pixel].w[w] = 0;
          hf->neu[w].w[pixel] = 0;
        } else {
          hf->neu[pixel].w[w] += hf->td[w];
          hf->neu[w].w[pixel] += hf->td[w];
        }
      }
    }
  }
}

void train(Hopfield* hf) {
  for ()
}


void readFromFile(Hopfield*);
void runHopfield() {
  Hopfield hf;
  initHopfield(hf);

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
  while ((read = getline(&line, &length, filePtr)) != -1) {
    for (int i = 0; i < 10; i++) {
      if (line[i] == '.') {
        CVPush(&hf->td[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->td[image], 1);
      } else if (line[0] == '-') {
        image++;
        printf("Training %d read\n", image);
      }
    }
	}
  hf->numTDImg = image;
  image = 0;
  while ((read = getline(&line, &length, filePtr)) != -1) {
    for (int i = 0; i < 10; i++) {
      if (line[i] == '.') {
        CVPush(&hf->pd[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->pd[image], 1);
      } else if (line[0] == '-') {
        image++;
        printf("Problem %d read\n", image);
      }
    }
  }
  hf->numPDImg = image;
  if (DEBUG) {
    for (int i = 0; i < 10; i++) {
      CVPrintAsChar(&hf->td[i]);
    }
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

void readFromConsole(KMeans* km) {
	double x, y;
	char ch;
	scanf("%d", &km->clusters);
	scanf("%lf%c%lf", &x, &ch, &y);
	km->maxX = x;
	km->minX = x;
	km->maxY = y;
	km->minY = y;
	DVPush(&km->points, x);
	DVPush(&km->points, y);
	while(scanf("%lf%c%lf", &x, &ch, &y) != EOF) {
		DVPush(&km->points, x);
		DVPush(&km->points, y);
		if (x > km->maxX) { km->maxX = x; }
		if (x < km->minX) { km->minX = x; }
		if (y > km->maxY) { km->maxY = y; }
		if (y < km->minY) { km->minY = y; }
	}
	// Get number of clusters
}

int main() {
  srand(time(0));
	KMeansHandler();
	return (0);
}
