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

double k_de[4] = {0.25, 0.25, 0.5, 0.5};
double k_te[4] = {0.12, 0.25, 0.12, 0.25};

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
  double theta;
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
  PVector input_r;
  PVector input_c;
  double u[2][4];
  double w[4];
  double output[2];
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

double aggr_min(double u_de, double u_te) {
  return min(u_de, u_te);
}

double aggr_prod(double u_de, double u_te) {
  return u_de * u_te;
}

void calc_u(Fuzzy* fl, double d_e, double theta_e) {
  for (int i = 0; i < 4; i++) {
    double v_l = k_de[i] * d_e + k_te[i] * theta_e;
    double v_r = k_de[i] * d_e - k_te[i] * theta_e;
    fl->u[0][i] = v_l;
    fl->u[1][i] = v_r;
  }
}

void calc_w(Fuzzy* fl, double u_de_small, double u_de_large, double u_theta_small, double u_theta_large) {
  fl->w[0] = aggr_prod(u_de_small, u_theta_small);
  fl->w[1] = aggr_prod(u_de_small, u_theta_large);
  fl->w[2] = aggr_prod(u_de_large, u_theta_small);
  fl->w[3] = aggr_prod(u_de_large, u_theta_large);
}

void calc_output(Fuzzy* fl) {
  double v_l_n = 0, v_l_d = 0, v_r_n = 0, v_r_d = 0;
  for (int i = 0; i < 4; i++) {
    v_l_n += fl->w[i] * fl->u[0][i];
    v_l_d += fl->w[i];
    v_r_n += fl->w[i] * fl->u[1][i];
    v_r_d += fl->w[i];
  }
  fl->output[0] = v_l_n / v_l_d;
  fl->output[1] = v_r_n / v_r_n;
}

void readFromFile(Fuzzy*);
void readFromConsole(Fuzzy* fl);
void run() {
  Fuzzy fl;
  initPVector(&fl.input_r);
  initPVector(&fl.input_c);
  //readFromFile(&fl);
  readFromFile(&fl);
  for (int i = 0; i < fl.input_r.size; i++) {
    double d_e = euclidean(fl.input_r.d[i], fl.input_c.d[i]);
    double theta_e = fl.input_r.d[i].theta - fl.input_c.d[i].theta;
    double u_de_small = calc_u_de_small(d_e);
    double u_de_large = calc_u_de_large(d_e);
    double u_theta_small = calc_u_theta_small(theta_e);
    double u_theta_large = calc_u_theta_large(theta_e);
    calc_u(&fl, d_e, theta_e);
    calc_w(&fl, u_de_small, u_de_large, u_theta_small, u_theta_large);
    calc_output(&fl);
    if (DEBUG) {
      printf("--------------\n");
      printf("%lf,%lf,%lf,%lf\n", u_de_small, u_de_large, u_theta_small, u_theta_large);
      printf("d_e: %lf, theta_e: %lf\n", d_e, theta_e);
      printf("v_l u1: %lf, u2: %lf, u3: %lf, u4: %lf\n", fl.u[0][0], fl.u[0][1], fl.u[0][2], fl.u[0][3]);
      printf("v_r u1: %lf, u2: %lf, u3: %lf, u4: %lf\n", fl.u[1][0], fl.u[1][1], fl.u[1][2], fl.u[1][3]);
      printf("w1: %lf, w2: %lf, w3: %lf, w4: %lf\n", fl.w[0], fl.w[1], fl.w[2], fl.w[3]);
    }
    printf("%lf,%lf\n", fl.output[0], fl.output[1]);
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
      double xr, yr, tr, xc, yc, tc;
      sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf", &xr, &yr, &tr, &xc, &yc, &tc);
      Point pr = {xr, yr, tr};
      Point pc = {xc, yc, tc};
      PVPush(&fl->input_r, pr);
      PVPush(&fl->input_c, pc);
  }
  if (DEBUG) {
    for (int i = 0; i < fl->input_r.size; i++) {
      printf("xr: %lf, yr: %lf, tr: %lf, xc: %lf, yc: %lf, tc: %lf\n", fl->input_r.d[i].x, fl->input_r.d[i].y, fl->input_r.d[i].theta, fl->input_c.d[i].x, fl->input_c.d[i].y, fl->input_c.d[i].theta);
    }
    printf("End read\n");
  }
  fclose(filePtr);
  if (line) {
      free(line);
  }
}

void readFromConsole(Fuzzy* fl) {
  char line[1000];
  while (scanf("%s", line) != EOF) {
      double xr, yr, tr, xc, yc, tc;
      sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf", &xr, &yr, &tr, &xc, &yc, &tc);
      Point pr = {xr, yr, tr};
      Point pc = {xc, yc, tc};
      PVPush(&fl->input_r, pr);
      PVPush(&fl->input_c, pc);
  }
  if (DEBUG) {
    for (int i = 0; i < fl->input_r.size; i++) {
      printf("xr: %lf, yr: %lf, tr: %lf, xc: %lf, yc: %lf, tc: %lf\n", fl->input_r.d[i].x, fl->input_r.d[i].y, fl->input_r.d[i].theta, fl->input_c.d[i].x, fl->input_c.d[i].y, fl->input_c.d[i].theta);
    }
    printf("End read\n");
  }
}

// END SOM

int main() {
  srand(time(0));
  run();
	return (0);
}
