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

void CVPrintAsChar(CVector* vector, bool last) {
  for (int row = 0; row < N_X * N_Y; row += N_X) {
    for (int col = 0; col < N_X; col++) {
      if (vector->d[row + col] == -1) {
        printf(".");
      } else {
        printf("*");
      }
    }
    printf("\n");
  }
	if (!last) {
		printf("-\n");
	}
}

void initCVector(CVector* vector) {
  vector->size = 0;
}

void CVPush(CVector* vector, char val) {
    vector->d[vector->size] = val;
    vector->size++;
}

void CVCopy(CVector* vector, CVector copiedFrom) {
	for (int i = 0; i < copiedFrom.size; i++) {
		vector->d[i] = copiedFrom.d[i];
		vector->size = copiedFrom.size;
	}
}

// END CVECTOR


typedef struct {
  int w[N_X * N_Y];
  int x;
} Neuron;

typedef struct {
  Neuron neu[N_X * N_Y];
  CVector td[10];
  int numTDImg;
  CVector pd[10];
  int numPDImg;
} Hopfield;

void HFSetOutToZero(Hopfield* hf) {
	for (int node = 0; node < N_X * N_Y; node++) {
		hf->neu[node].x = 0;
	}
}

void readFromConsole(Hopfield*);
void readFromFile(Hopfield*);
void initHopfield(Hopfield* hf) {
  // Set neuron output state
  for (int i = 0; i < 10; i++) {
    initCVector(&hf->td[i]);
    initCVector(&hf->pd[i]);
  }
  readFromFile(hf);
	// For each node
	for (int node = 0; node < N_X * N_Y; node++) {
		// Set the initial output to zero
		hf->neu[node].x = 0;
		// Set each weight of given node
  	for (int weight = 0; weight < N_X * N_Y; weight++) {
			// To the sum of all pixels of each image for current weight
			// multiplied with the weight of current node
  		for (int image = 0; image < hf->numTDImg; image++) {
        if (node == weight) {
          hf->neu[node].w[weight] = 0;
        } else {
          hf->neu[node].w[weight] += hf->td[image].d[node] * hf->td[image].d[weight];
        }
      }
    }
  }
  /*
	for (int y = 0; y < N_Y; y++) {
		for (int x = 0; x < N_X; x++) {
			printf("N%d: ", y * N_Y + x);
			for (int weight = 0; weight < N_X * N_Y; weight++) {
				printf("%d ", hf->neu[y * N_Y + x].w[weight]);
			}
		}
		printf("\n");
	}
	printf("\n");
  */
}

// Return -1 if no match is found, return index of image if
// similarity is over 70%
bool compare(Hopfield* hf /*, CVector* input*/) {
	double similarCount[10] = {0};
	for (int image = 0; image < hf->numTDImg; image++) {
		for (int i = 0; i < N_X * N_Y; i++) {
			if (hf->neu[i].x == hf->td[image].d[i])	{
				similarCount[image]++;
			}
		}
	}
	int maxPercent = 0;
	int maxIndex = 0;
	for (int image = 0; image < hf->numTDImg; image++) {
		if ((similarCount[image] / (N_X * N_Y)) > maxPercent) {
			maxPercent = similarCount[image] / (N_X * N_Y);
			maxIndex = image;
		}
	}
	if (maxPercent < 0.7) {
		return -1;
	} else {
    /*
    printf("CURRENT SUGGESTED:\n");
    CVPrintAsChar(&hf->td[maxIndex], false);
    printf("SET AS:\n");
    CVPrintAsChar(input, false);
    */
		return maxIndex;
	}
}

void flip(Hopfield* hf) {
	for (int i = 0; i < N_X * N_Y / 15; i++) {
    int oldVal = hf->neu[rand() % (N_X * N_Y)].x;
    int newVal = oldVal * -1;
    int randNum = rand() % (N_X * N_Y);
    printf("old: %d, new: %d\n, pos: %d", oldVal, newVal, randNum);
		hf->neu[randNum].x = newVal;
	}
}

int normalize(int value) {
  if (value >= 0) {
    return 1;
  } else if (value < 0) {
    return -1;
  } else {
    //printf("Output was 0, what to do?!?!?\n");
    return 0;
  }
}

void checkPattern(Hopfield* hf) {
	// Check for each image
	for (int image = 0; image < hf->numPDImg; image++) {
		bool hasChanged = true;
		CVector input;
		initCVector(&input);
		CVCopy(&input, hf->pd[image]);
    //printf("STARTING WITH:\n");
    //CVPrintAsChar(&input, false);
		// Continue until stable
		while (hasChanged){
			hasChanged = false;
			// Calculate output for given image
			for (int neu = 0; neu < N_X * N_Y; neu++) {
				int prevX = hf->neu[neu].x;
        if (hf->neu[neu].x != 0) {
          prevX = normalize(prevX);
        }
				hf->neu[neu].x = 0;
				for (int weight = 0; weight < N_X * N_Y; weight++) {
					hf->neu[neu].x += hf->neu[neu].w[weight] * input.d[weight];
				}
				hf->neu[neu].x = normalize(hf->neu[neu].x);
				// If different output from previous, continue the while-loop
				if (prevX != hf->neu[neu].x) {
					hasChanged = true;
				}
			}
			// Copy new output to input
			for (int i = 0; i < N_X * N_Y; i++) {
				input.d[i] = hf->neu[i].x;
				input.size = N_X * N_Y;
			}
      /*
  	for (int y = 0; y < N_Y; y++) {
  		for (int x = 0; x < N_X; x++) {
  			printf("N%d: %d ", y * N_Y + x, input.d[y * N_Y + x]);
  		}
  		printf("\n");
  	}
  	printf("\n");
    break;
    */
		}
		int matchingPattn = compare(hf);
		// If no matching pattern flip 10% of the output
		if (matchingPattn == -1) {
			image--;
			flip(hf);
		// More than 70% match, assume it is the given training pattern
		} else {
			bool last = false;
			if ((image + 1) == hf->numPDImg) {
				last = true;
			}
			CVPrintAsChar(&hf->td[matchingPattn], last);
		}
	}
}

void runHopfield() {
  Hopfield hf;
  initHopfield(&hf);
	checkPattern(&hf);

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
    for (int i = 0; i < read - 1; i++) {
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
  printf("TDIMG: %d\n", image);
  done = false;
  image = 0;
  for (int i = 0; i < read - 1; i++) {
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
    for (int i = 0; i < read - 1; i++) {
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
      CVPrintAsChar(&hf->td[i], false);
    }
    for (int i = 0; i < hf->numPDImg; i++) {
      CVPrintAsChar(&hf->pd[i], false);
    }
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

void readFromConsole(Hopfield* hf) {
  char line[20];

  int image = 0;
  bool done = false;
  while ((scanf("%s", line) != EOF) && !done) {
    for (int i = 0; i < N_X; i++) {
      if (line[i] == '.') {
        CVPush(&hf->td[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->td[image], 1);
      } else if (line[0] == '-' && line[1] == '-') {
        image++;
        //printf("Training done\n");
        done = true;
        break;
      } else {
        image++;
        //printf("Training %d read\n", image);
        break;
      }
    }
	}
  hf->numTDImg = image;
  //printf("TDIMG: %d\n", image);
  done = false;
  image = 0;
  for (int i = 0; i < N_X; i++) {
    if (line[i] == '.') {
      CVPush(&hf->pd[image], -1);
    } else if (line[i] == '*') {
      CVPush(&hf->pd[image], 1);
    } else if (line[0] == '-') {
      image++;
      //printf("Problem %d read\n", image);
      break;
    }
  }
  while ((scanf("%s", line) != EOF) && !done) {
    for (int i = 0; i < N_X; i++) {
      if (line[i] == '.') {
        CVPush(&hf->pd[image], -1);
      } else if (line[i] == '*') {
        CVPush(&hf->pd[image], 1);
      } else if (line[0] == '-') {
        image++;
        //printf("Problem %d read\n", image);
        break;
      }
    }
  }
  image++;
  //printf("PDIMG: %d\n", image);
  hf->numPDImg = image;
  if (0) {
    for (int i = 0; i < hf->numTDImg; i++) {
      CVPrintAsChar(&hf->td[i], false);
    }
    for (int i = 0; i < hf->numPDImg; i++) {
      CVPrintAsChar(&hf->pd[i], false);
    }
  }
}

int main() {
  srand(time(0));
  runHopfield();
	return (0);
}
