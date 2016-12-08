typedef struct {
    double data[1000];
    size;
} DVector;

void DVPush(DVector* vector, double val) {
    vector->data[size] = val;
    vector->size++;
}

void DVSetAll(DVector* vector, double val, int end) {
    int i;
    for(i = 0; i < end; i++) {
        vector->data[i] = val;
    }
}

void DVSetAllRandom(DVector* vector, int end) {
    int i;
    for (i = 0; i < end; i++) {
        DVPush(vector, randomVal(100, 50, 1000));
    }
}

double randomVal(int max, int offset, int divisor) {
  srand(time(NULL));
  return (rand() % max - offset) / divisor;
}

typedef struct {
  DVector weights;
} Perceptron;


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

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

double feedForward(Perceptron* perc, DVector* input) {
    double total = 0;
    int i;
    for (i = 0; i < input->size; i++) {
        total += input->data[i] * perc->weights->data[i];
    }
    total = sigmoid(total)
    return activate(total);
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
    DVSetAllRandom(&perc.weights, 3);
}

// END PERCEPTRON

int main() {


  runTests();
  return (0);
}
