#include <time.h>
#include <stdlib.h>

#define CAPACITY 1000

typedef struct {
    int size;
    int cap;
    int* data;
} Vector;

void initVector(Vector* vector, int initSize) {
   vector->size = 0;
   vector->cap = CAPACITY; 
   vector->data = (int *)malloc(initSize * sizeof(int));
}

void init2DVector(Vector* vector, int size1, int size2) {
    int i;
    for (i = 0; i < size1; i++) {
        vector.data
    }
}

// ----- END VECTOR -----

typedef struct {
    int size;
    int cap;
    int** data;
} 2DVector;


void init2DVector(Vector* vector, int size1, int size2) {
    vector->size = 0;
    vector->cap = CAPACITY; 
    vector->data = malloc(size1 * sizeof(int*));
    int i;
    for (i = 0; i < )
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