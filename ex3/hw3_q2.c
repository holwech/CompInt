#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
#define ANS 1
#define NMAX 10
#define MAX_POINTS 1000
#define NEURONS_X 20 
#define NEURONS_Y 10 

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
  double w[NEURONS_X * NEURONS_Y];
  double x;
} Neuron;

typedef struct {
  Neuron neu[NEURONS_X * NEURONS_Y];
} Hopfield;

void initHopfield(Hopfield* hf) {
  char inputs[NEURONS_X * NEURONS_Y];
  readFromFile(&input);
  // Set neuron output state 
  for (int input = 0; input < NEURONS_X * NEURONS_Y; input++) {
    hf->neu[input].x = inputs[input]; 
  }
  // Calculate initial weights
  for (int n_i = 0; n_i < NEURONS_X * NEURONS_Y; n_i++) {
    double x_i = hf.neu[n_i].x;
    for (int n_j = 0; n_j < NEURONS_X * NEURONS_Y; n_j++) {
      double x_j = hf.neu[n_j].x;
      if (n_i == n_j) {
        hf.neu[n_i].w[weight] = 0
      } else {
        hf.neu[col][row].w[weight] = x_i * x_j;
      }
    }
  }
}

// END KMEANS


void readFromFile(KMeans* km) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
	// Get number of clusters
	read = getline(&line, &length, filePtr);
	sscanf(line, "%d", &km->clusters);
	read = getline(&line, &length, filePtr);
	double x, y;
	char ch;
	sscanf(line, "%lf%c%lf", &x, &ch, &y);
	km->maxX = x;
	km->minX = x;
	km->maxY = y;
	km->minY = y;
	DVPush(&km->points, x);
	DVPush(&km->points, y);
  while ((read = getline(&line, &length, filePtr)) != -1) {
    sscanf(line, "%lf%c%lf", &x, &ch, &y);
		DVPush(&km->points, x);
		DVPush(&km->points, y);
		if (x > km->maxX) { km->maxX = x; }
		if (x < km->minX) { km->minX = x; }
		if (y > km->maxY) { km->maxY = y; }
		if (y < km->minY) { km->minY = y; }
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
