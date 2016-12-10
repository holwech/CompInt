#include <stdio.h>
#include <math.h>

typedef struct {
    double data[1000];
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

double randomVal(int max, int offset, int divisor) {
  srand(time(0));
  return (rand() % max - offset) / divisor;
}

void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        double r = randomVal(100, 50, 1000);
        DVPush(vector, r);
    }
}


// END DVECTOR

typedef struct {
  int data[1000];
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

double feedForward(Perceptron* perc, DVector* input) {
    double total = 0;
    int i;
    for (i = 0; i < input->size; i++) {
        total += input->data[i] * perc->weights.data[i];
    }
    total = sigmoid(total);
    return activate(total);
}

void train(Perceptron* perc, DVector* inputs, int answer) {
  double LR = 0.01;
  double ff = feedForward(perc, inputs);
  float error = answer - ff;
  if (1) {
    printf("Input: %lf, %lf, %lf\n", inputs->data[0], inputs->data[1], inputs->data[2]);
    int j;
    for (j = 0; j < inputs->)
    printf("Before weights: ");
    int p;
    for (p = 0; p < perc->weights.size; p++) {
      printf("%lf, ", perc->weights[p]);
    }
    printf("\n");
    printf("Answer: %d ff: %lf Error: %lf\n", answer, ff, error);
  }
  int i;
  for(i = 0; i < perc->weights.size; i++) {
    perc->weights.data[i] = LR * error * inputs->data[i];
  }
  if (1) {
    printf("After weights: ");
    int p;
    for (p = 0; p < perc->weights.size; p++) {
      printf("%lf ", perc->weights[p]);
    }
    printf("\n");
  }
}

int activate(double val) {
    if (val > 0) {
        return 1;
    } else {
        return 0;
    }
}

void runPerceptron() {
    Perceptron perc;
    initDVector(&perc.weights);
    DVSetAllRandom(&perc.weights, 3);
}

// END PERCEPTRON

void readData(DVector* input) {
  int stop = 0;
  char str[100];
  int i;
  while(stop == 0)  {
    double x, y, a;
    char ch;
    scanf("%lf%c%lf%c%lf", &x, &ch, &y, &ch, &a);
    if(0) {
      printf("%lf %lf %lf\n", x, y, a);
    }
    DVPush(input, x);
    DVPush(input, y);
    DVPush(input, a);
    if (x == 0.0 && y == 0.0 && a == 0.0) {
      stop = 1;
    }
  }
}

int main() {
  DVector input;
  initDVector(&input);
  readData(&input);

  return (0);
}
