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
#define CALC_N 2

double alpha = 0.3;
double sigma = 1.0;

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
    int d[100];
    int size;
} IVector;


void initIVector(IVector* vector) {
  vector->size = 0;
}

void IVPush(IVector* vector, char val) {
    vector->d[vector->size] = val;
    vector->size++;
}

void IVShuffle(IVector* vector) {
    for (int i = 0; i < vector->size; i++) {
        int randPos1 = rand() % vector->size;
        int randPos2 = rand() % vector->size;
        int temp1 = vector->d[randPos1];
        vector->d[randPos1] = vector->d[randPos2];
        vector->d[randPos2] = temp1;
    }
}

IVector shuffledIndex(int length) {
    IVector vector;
    initIVector(&vector);
    for (int i = 0; i < length; i++) {
        IVPush(&vector, i);
    }
    IVShuffle(&vector);
    return vector;
}


// END IVECTOR

typedef struct {
    int d[1000];
    int size;
} BIVector;


void initBIVector(BIVector* vector) {
  vector->size = 0;
}

void BIVPush(BIVector* vector, char val) {
    vector->d[vector->size] = val;
    vector->size++;
}

void BIVShuffle(BIVector* vector) {
    for (int i = 0; i < vector->size; i++) {
        int randPos1 = rand() % vector->size;
        int randPos2 = rand() % vector->size;
        int temp1 = vector->d[randPos1];
        vector->d[randPos1] = vector->d[randPos2];
        vector->d[randPos2] = temp1;
    }
}

BIVector shuffledBIndex(int length) {
    BIVector vector;
    initBIVector(&vector);
    for (int i = 0; i < length; i++) {
        BIVPush(&vector, i);
    }
    BIVShuffle(&vector);
    return vector;
}

// END BIVECTOR

typedef struct {
    char d[3000];
    int size;
} CVector;


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
  double x;
  double y;
  double delta;
} Point;

double euclidean(Point p1, Point p2) {
    double distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    return distance;
}

void PPrint(Point point) {
    printf("x: %lf, y: %lf \n", point.x, point.y);
}

// END POINT
typedef struct {
    Point d[1000];
    int size;
} PVector;

void initPVector(PVector* vector) {
    vector->size = 0;
}

void PVPush(PVector* vector, Point point) {
    vector->d[vector->size] = point;
    vector->size++;
}

void PVScale(PVector* vector, double max, double min) {
    for (int i = 0; i < vector->size; i++) {
        vector->d[i].x = vector->d[i].x / (max - min);
        vector->d[i].y = vector->d[i].y / (max - min);
    }
}

void PVShuffle(PVector* vector) {
    for (int i = 0; i < vector->size; i++) {
        int randPos1 = rand() % vector->size;
        int randPos2 = rand() % vector->size;
        Point temp1 = vector->d[randPos1];
        vector->d[randPos1] = vector->d[randPos2];
        vector->d[randPos2] = temp1;
    }
}

void PVPrint(PVector* vector) {
    for (int p = 0; p < vector->size; p++) {
        printf("{%lf, %lf} \n", vector->d[p].x, vector->d[p].y);
    }
}


// END POINT ARR

typedef struct {
  PVector input1;
  PVector input2;
} Fuzzy;

// END FUZZY

double min(double val1, double val2) {
  if (val1 < val2) {
    return val1;
  } else {
    return val2;
  }
}

double max(double val1, double val2) {
  if (val1 > val2) {
    return val1;
  } else {
    return val2;
  }
}

double calc_u_de_small(double d_e) {
  return min(max(0, (d_e - 80.0) / -160.0), 1);
}

double calc_u_de_large(double d_e) {
  return 1 - calc_u_de_small(d_e);
}

double calc_u_theta_small(double theta) {
  return (-fabs(theta) + 180) / 180;
}

double calc_u_theta_large(double theta) {
  return 1 - calc_u_theta_small(theta);
}

void run() {
  Fuzzy fl;
  initPVector(&fl.input1);
  initPVector(&fl.input2);
  for (int i = 0; i < fl.size; i++) {
    double d_e = euclidean(fl.input1.d[i], fl.input2.d[i]);
    theta_e = theta_r - theta_c;
    u_de_small = calc_u_theta_small(d_e);
    u_de_large = calc_u_theta_large(d_e);
    u_theta_small = calc_u_theta_small(theta_e);
    u_theta_large = calc_u_theta_large(theta_e);
    printf("%lf,%lf,%lf,%lf\n", u_de_small, u_de_large, u_theta_small, u_theta_large);
  }
}

void readFromFile(Fuzzy* fl) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  while ((read = getline(&line, &length, filePtr)) != -1) {
      int xr, yr, tr, xc, yc, tc;
      sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf", &xr, &yr, %tr, &xc, &yc, &tc);
      Point pr = {xr, yr, tr};
      Point pc = {xc, yc, tc};
      PVPush(&fl->input1, pr);
      PVPush(&fl->input2, pc);
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

// END SOM

int main() {
  srand(time(0));
	return (0);
}
