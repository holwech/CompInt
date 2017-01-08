#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define CAPACITY 1000

void assert(int expr) {
  if (expr) {
    printf(" ...OK\n");
  } else {
      printf(" ...FAILED\n");
  }
}

typedef struct Vector {
    int size;
    int cap;
    void** data;
} Vector;

void initVector(Vector* vector, int initSize) {
   vector->size = 0;
   vector->cap = initSize;
   vector->data = malloc(initSize * sizeof(void*));
}

void test_initVector() {
  printf("%s\n", "----- Test initVector -----");
  int initSize = 10;
  Vector vector;
  initVector(&vector, initSize);
  printf("Size");
  assert(vector.size == 0);
  printf("Capacity");
  assert(vector.cap == initSize);
}

void vectorPush(Vector* vector, void* data) {
  if(vector->size == vector->cap) {
    // Add a resize here later
    printf("%s\n", "This vector is full!");
    return;
  }
  vector->data[vector->size] = data;
  vector->size++;
}

void test_vectorPush() {
  printf("%s\n", "----- Test vectorPush -----");
  Vector vector1;
  int initSize = 10;
  initVector(&vector1, initSize);
  int* values = malloc(11 * sizeof(int));
  int j;
  for (j = 0; j <= 11; j++)  {
    values[j] = 22;
  }
  vectorPush(&vector1, &values[0]);
  printf("- Testing for 1 push\n");
  printf("Push int");
  assert(*((int*)vector1.data[0]) == 22);
  printf("Push int size");
  assert(vector1.size == 1);
  printf("Push int cap");
  assert(vector1.cap == 10);
  vectorPush(&vector1, &values[1]);
  printf("- Testing for 2 push\n");
  printf("Push int");
  printf("%d\n", *((int*)vector1.data[1]));
  printf("Push int size");
  assert(vector1.size == 2);
  printf("Push int cap");
  assert(vector1.cap == 10);
  int i;
  printf("- Overflowing vector\n");
  for (i = vector1.size; i <= vector1.cap; i++) {
    vectorPush(&vector1, &values[i]);
  }
  printf("Overflow");
  assert(vector1.size == 10);
  printf("Last value");
  assert(*((int*)vector1.data[9]) == 22);
}

// Set all values in the vector to a spesific value
// If random is true, all values will be set to a low random value
void vectorSetAllValues(Vector* vector, void* value) {
  if (vector->size != 0) {
    printf("Size not 0, setAllValues was not run\n");
    return;
  }
  int i;
  for(i = 0; i < vector->cap; i++) {
    vectorPush(vector, value);
  }
}


// Dereferencing to something *((int*)vector->data[0])
void test_setAllValues() {
  printf("----- Test setAllValues -----\n");
  Vector vector;
  int* allVal = malloc(sizeof(int));
  *allVal = 22;
  initVector(&vector, 10);
  vectorSetAllValues(&vector, allVal);
  printf("Printing all values: {");
  int allSet = 1;
  int i;
  for (i = 0; i < vector.size; i++) {
    printf("%d, ", *((int*)vector.data[i]));
    if (*((int*)vector.data[i]) != *allVal) {
      allSet = 0;
    }
  }
  printf("}\n");
  printf("Set all values");
  assert(allSet == 1);
}

void printVector(Vector* vector, char type) {
  printf("%s\n", "----- Vector -----");
  printf("Size: %d\n", vector->size);
  printf("Cap: %d\n", vector->cap);
  printf("Data: ");
  printf("Not working atm\n");
}

void print(Vector* vector, char type) {
  if (type == 'i') {
    int i;
    for (i = 0; i < vector->size; i++) {
      printf("%d ", *((int*)vector.data[i]);
    }
    printf("\n");
  } else if (type == 'c') {
    int i;
    for (i = 0; i < vector->size; i++) {
      printf("%c ", *((char*)vector.data[i]);
    }
    printf("\n");
  }
}

void runTests() {
  test_initVector();
  test_vectorPush();
  test_setAllValues();
}

// END VECTOR

typedef struct {
    Vector biases;
    Vector weights;
    Vector sizes;
    int layers;
} Network;

void initNetwork(Network* network, Vector* sizes, int* weightVals, *int biasVals) {
    initVector(&network->biases, 1000);
    initVector(&network->weights, 1000);
    initVector(&network->sizes, 1000);
    int i;
    for(i = 0; i < sizes->size; i++) {
      vectorPush(&network->sizes, &sizes->data[i]);
    }
    network->layers = sizes->size;
    vectorPush(&network->weights, weightVals);
    vectorPush(&network->weights, weightVals + 1);
    vectorPush(&network->biases, biasVals);
    vectorPush(&network->biases, biasVals + 1);
}

double sigmoid(double input) {
  return 1.0 / (1.0 + exp(-input));
}

void runNetwork() {
  Network network;
  Vector sizes;
  initVector(&sizes, 1000);
  nodesPerLayer = {2, 2};
  vectorPush(&sizes, &nodesPerLayer[0]);
  vectorPush(&sizes, &nodesPerLayer[1]);
  double weightVals = {0.001, 0.0014};
  double biasVals = {-0.0011, 0.0001};

  initNetwork(&network, &sizes, &rValWeightsMem, &biasVals);

}

// END NETWORK


typedef struct {
  Vector weights;
} Perceptron;

void initPerceptron(*Perceptron perc, double* numWeights) {
    vectorPush(&perc->weights, numWeights);
    vectorPush(&perc->weights, numWeights + 1);

}

int feedForward(Perceptron* perc, Vector* input) {
  float total = 0;
  int i;
  for(i = 0; i < input.size; i++) {
    total += sigmoid()
    total += *((double*)input->data[i]) * *((double*)perc->weights->data[i]);
  }
  if (total > 0) {
    return 1;
  } else {
    return 0;
  }
}


void train(Vector* perc, Vector* inputs, int answer) {
  double LR = 0.01;
  int ff = feedForward(perc, inputs);
  float error = answer - ff;
  int i;
  for(i = 0; i < perc->weights->size; i++) {
    *((double*)perc->weights->data[0]) = LR * error * *((int*)inputs->data[i]);
  }
}

float randomVal() {
  srand(time(NULL));
  return (rand() % 100 - 50) / 1000;
}

void runPerceptron() {
  Perceptron perc;
  initVector(&perc.weights, 1000);
  float numWeights = {randomVal(), randomVal()};

  initPerceptron(&perc, numWeights);
}

// END PERCEPTRON

int main() {


  runTests();
  return (0);
}
