#include <time.h>
#include <stdlib.h>

#define CAPACITY 1000

typedef struct {
    int size;
    int cap;
    void* data;
} Vector;

void initVector(Vector* vector, int initSize) {
   vector->size = 0;
   vector->cap = initSize;
   vector->data = (void *)malloc(initSize * sizeof(void));
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

// ----- END VECTOR -----

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


// ----- END 2DVECTOR

typedef struct {
    Vector biases;

    Vector sizes;
    int layers;
} Network;

void initNetwork(Network* network, Vector* sizes) {
    srand((unsigned int) time(NULL));
    initVector(&network.biases, sizes.size - 1);

}
