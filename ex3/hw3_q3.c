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
  int index;
  IVector closest;
} Point;

double pDistance(Point p1, Point p2) {
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
    PVector points;
    PVector cities;
    double maxX, maxY, minX, minY, max, min, diffX, diffY;
} SOM;

void initSOM(SOM* som) {
   initPVector(&som->points);
   initPVector(&som->cities);
   for (int p = 0; p < som->points.size; p++) {
       initIVector(&som->points.d[p].closest);
   }
   som->maxX = 0;
   som->maxY = 0;
   som->minX = 10000000;
   som->minY = 10000000;
   som->max = 0;
   som->min = 10000000;
   sigma = som->cities.size / 2;
}

void place(SOM* som) {
    Point center = {som->minX + (som->diffX / 2), som->minY + (som->diffY / 2)};
    int a = som->diffX / 2;
    int b = som->diffY / 2;
    int numPoints = 1.4 * som->cities.size;
    //printf("center: %lf, %lf, a: %d, b: %d, numPoints: %d \n", center.x, center.y, a, b, numPoints);
    for (int p = 0; p < numPoints; p++) {
       Point newPoint = {center.x + a * cos((2.0 * 3.14 / numPoints) * p),center.y + b * sin((2.0 * 3.14 / numPoints) * p)};
       PVPush(&som->points, newPoint);
    }
}

int getIndex(int index, int size) {
    if (index < 0) {
        return size + index;
    } else {
        return index % size;
    }
}

double fFormula(double d, double sigma) {
    return exp((-pow(d, 2)) / pow(sigma, 2));
}

Point newPos(Point point, Point pullPoint) {
    int d = pDistance(point, pullPoint);
    int f = fFormula(d, sigma);
    Point newPoint = {point.x + alpha * f * (pullPoint.x - point.x), point.y + alpha * f * (pullPoint.y - point.y)};
    return newPoint;
}

void move(SOM* som, int pointIndex, int cityIndex) {
    som->points.d[pointIndex] = newPos(som->points.d[pointIndex], som->cities.d[cityIndex]);
    // For each neighbour, move
    for (int n = 1; n < CALC_N + 1; n++) {
        Point nPointRight = som->points.d[getIndex((pointIndex + n), som->points.size)];
        Point nPrevPointRight = som->points.d[getIndex((pointIndex + n - 1), som->points.size)];
        Point nPointLeft = som->points.d[getIndex((pointIndex - n), som->points.size)];
        Point nPrevPointLeft = som->points.d[getIndex((pointIndex - n + 1), som->points.size)];
        som->points.d[getIndex((pointIndex + n), som->points.size)] = newPos(nPointRight, nPrevPointRight);
        som->points.d[getIndex((pointIndex - n), som->points.size)] = newPos(nPointLeft, nPrevPointLeft);
    }
}

void updateAlpha() {
    alpha -= 0.001;
    if (alpha < 0.01) {
        alpha = 0.01;
    }
}

void train(SOM* som) {
    //PVScale(&som->cities);
    place(som);
    //PVPrint(&som->points);
    int timeStart = (int)time(NULL);
    while((int)time(NULL) - timeStart < 200) {
        BIVector shuffle = shuffledBIndex(som->cities.size);
        for (int city = 0; city < som->cities.size; city++) {
            double minDistance = 10000000;
            double minIndex = 0;
            for (int p = 0; p < som->points.size; p++) {
                double dist = pDistance(som->cities.d[shuffle.d[city]], som->points.d[p]);
                if (dist < minDistance) {
                    minDistance = dist;
                    minIndex = p;
                }
            }
            move(som, minIndex, shuffle.d[city]);
        }
        updateAlpha();
    }
    for (int city = 0; city < som->cities.size; city++) {
        double minDistance = 10000000;
        int minIndex = 0;
        for (int p = 0; p < som->points.size; p++) {
            double dist = pDistance(som->cities.d[city], som->points.d[p]);
            if (dist < minDistance) {
                minDistance = dist;
                minIndex = p;
            }
        }
        IVPush(&som->points.d[minIndex].closest, city + 1);
    }
    for (int p = 0; p < som->points.size; p++) {
        for (int city = 0; city < som->points.d[p].closest.size; city++) {
            printf("%d\n", som->points.d[p].closest.d[city]);
        }
    }
}


void readFromConsole(SOM* som);
void readFromFile(SOM* som);
void runSOM() {
    SOM som;
    initSOM(&som);
    readFromConsole(&som);
    train(&som);
}

void readFromFile(SOM* som) {
  FILE* filePtr;
  size_t length = 0;
  char* line = 0;
  int read;

  filePtr = fopen("input.txt", "r");
  if (filePtr == NULL) {
    printf("File open failed\n");
  }
  int index = 0;
  while ((read = getline(&line, &length, filePtr)) != -1) {
      int x, y;
      sscanf(line,"%d,%d,%d", &index, &x, &y);
      Point newPoint = {x, y, index};
      PVPush(&som->cities, newPoint);
      if (som->maxX < x) {
          som->maxX = x;
      }
      if (som->minX > x) {
          som->minX = x;
      }
      if (som->maxY < y) {
          som->maxY = y;
      }
      if (som->minY > y) {
          som->minY = y;
      }
      printf("%d, %lf, %lf\n", som->cities.d[index - 1].index, som->cities.d[index - 1].x, som->cities.d[index - 1].y);
  }
  if (som->maxX > som->maxY) {
      som->max = som->maxX;
  } else {
      som->max = som->maxY;
  }
  if (som->minX < som->minY) {
      som->min = som->minX;
  } else {
      som->min = som->minY;
  }
  som->diffX = som->maxX - som->minX;
  som->diffY = som->maxY - som->minY;
  printf("MaxX: %lf, MinX: %lf, MaxY: %lf, MinY: %lf, max: %lf, min: %lf, diffX: %lf, diffY: %lf\n", som->maxX, som->minX, som->maxY, som->minY, som->max, som->min, som->diffX, som->diffY);
  fclose(filePtr);
  if (line) {
      free(line);
  }
}


void readFromConsole(SOM* som) {
  char line[20];
  int index = 0;
  while (scanf("%s", line) != EOF) {
      int x, y;
      sscanf(line,"%d,%d,%d", &index, &x, &y);
      Point newPoint = {x, y, index};
      PVPush(&som->cities, newPoint);
      if (som->maxX < x) {
          som->maxX = x;
      }
      if (som->minX > x) {
          som->minX = x;
      }
      if (som->maxY < y) {
          som->maxY = y;
      }
      if (som->minY > y) {
          som->minY = y;
      }
      //printf("%d, %lf, %lf\n", som->cities.d[index - 1].index, som->cities.d[index - 1].x, som->cities.d[index - 1].y);
  }
  if (som->maxX > som->maxY) {
      som->max = som->maxX;
  } else {
      som->max = som->maxY;
  }
  if (som->minX < som->minY) {
      som->min = som->minX;
  } else {
      som->min = som->minY;
  }
  som->diffX = som->maxX - som->minX;
  som->diffY = som->maxY - som->minY;
  //printf("MaxX: %lf, MinX: %lf, MaxY: %lf, MinY: %lf, max: %lf, min: %lf, diffX: %lf, diffY: %lf\n", som->maxX, som->minX, som->maxY, som->minY, som->max, som->min, som->diffX, som->diffY);
}

// END SOM

int main() {
  srand(time(0));
  runSOM();
	return (0);
}
