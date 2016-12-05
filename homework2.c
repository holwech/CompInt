#include <time.h>
#include <stdlib.h>

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
   vector->data = (void *)malloc(initSize * sizeof(void));
}

void test_initVector() {
  printf("%s\n", " ----- Test initVector -----");
  struct Vector* vector;
  initVector(vector, 100);
  printf("%s\n", "Size");
  assert(vector->size == 0);
  printf("%s\n", "Capacity");
  assert(vector->cap == 100);
}

void VectorPush(Vector* vector, void* data) {
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
  struct Vector* vector1;
  initVector(vector1, 10);
  vectorPush(vector1, 22);
  printf("%s\n", "Push int");
  assert(vector1->data[0] == 22);
  printf("%s\n", "Push int size");
  assert(vector1->size == 1);
  printf("%s\n", "Push int cap");
  assert(vector1->size == 10);
  int i;
  for (i = 0; i <= 10; i++) {
    vectorPush(vector1, i + 3);
  }
  printf("%s\n", "Overflow");
  assert(vector1->size == 10);
}

// Set all values in the vector to a spesific value
// If random is true, all values will be set to a low random value
void setAllValues(Vector* vector, int value, int random) {
  if (random == 0) {
      int i;
      for (i = 0; i < vector->size; i++) {
        vector[i] = value;
      }
  } else {
    srand((unsigned int) time(NULL));
    int i;
    for (i = 0; i < vector.size; i++) {
      double r = ((rand() % 100) + 1) / 10000;
      vector->data[i] = r;
    }

  }
}

void test_setAllValues() {
  struct Vector* vector;
  initVector(vector, 10);
  setAllValues(vector, 22);
  int allSet = 1;
  int i;
  for (i = 0; i < vector; i++) {
    if (vector->data[i] != 22) {
      allSet = 0;
    }
  }
  printf("%s\n", "Set all values");
  assert(allSet == 0);
}

void printVector(Vector* vector, char type) {
  printf("%s\n", "----- Vector -----");
  printf("Size: %d\n", vector->size);
  printf("Cap: %d\n", vector->);
  printf("Data: ");
  int i;
  for (i = 0; i < vector->size; i++) {
    print(vector, type);
  }
}

void print(Vector* vector, char type) {
  if (type == 'i') {
    int i;
    for (i = 0; i < vector->size; i++) {
      printf("%d ", vector->data[i]);
    }
    printf("\n");
  } else if (type == 'c') {
    int i;
    for (i = 0; i < vector->size; i++) {
      printf("%c ", vector->data[i]);
    }
    printf("\n");
  }
}

// ----- END VECTOR -----
/*
typedef struct {
    int size;
    int cap;
    void** data;
} 2DVector;

void 2DVectorPush(2DVector* vector, void* data) {
  if(vector->size == vector->cap) {
    // Add a resize here later
    printf("%s\n", "This vector is full!");
    return;
  }
  vector->data[vector->size] = data;
  vector->size++;
}


void init2DVector(Vector* vector, int size1, int size2) {
    vector->size = 0;
    vector->cap = size1;
    vector->data = malloc(size1 * sizeof(void*));
    int i;
    for (i = 0; i < size1; i++) {
        Vector data;
        initVector(&data, size2);
        2DVectorPush(vector, data);
    }
}

*/
// ----- END 2DVECTOR
/*
typedef struct {
    Vector* biases;
    Vector* weights;
    Vector* sizes;
    int layers;
} Network;

void initNetwork(Network* network, Vector* sizes) {
    struct Vector* weights;
    int rows = sizes->size - 1;
    initVector(weights, rows);
    int row;
    for (row = 0; row < rows; row++) {
      struct Vector*
    }


    network.weights = weights;

    struct Vector* biases;
    setAllValues(biases, 0, 1);
    initVector(biases, sizes->size - 1);

    network.data = biases;

}
*/

int main() {
  test_initVector();
  test_vectorPush();
  test_setAllValues();
  return (0);
}
