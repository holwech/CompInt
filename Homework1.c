#include<stdio.h>
#include <string.h>

void multiplyBy2() {
  int num = 0;
  while (scanf("%d", &num) != EOF) {
    printf("%d\n", num*2);
  }
}

void bubbleSort() {
  char str[100];
  int success = scanf("%s", &str);
  int len = sizeof(str) / sizeof(str[0]);
  int i;
  int sorted = 0;
  int singlePrint = 1;
  while(!sorted) {
    sorted = 1;
    for(i = 0; i < len - 1; i++) {
      char temp;
      if (str[i] > str[i + 1]) {
        temp = str[i + 1];
        str[i + 1] = str[i];
        str[i] = temp;
        sorted = 0;
      }
    }
    if (!sorted) {
      singlePrint = 0;
      for (i = 0; i < len; i++) {
          printf("%c",str[i]);
        }
      printf("\n");
    }
    if (singlePrint) {
      for (i = 0; i < len; i++) {
          printf("%c",str[i]);
        }
      printf("\n");
    }
  }
}

void insertionSort() {
  char str[100];
  int success = scanf("%s", &str);
  int len = sizeof(str) / sizeof(str[0]);
  int forward;
  int sorted = 0;
  if (len == 0) {
    return;
  } else if (len == 1) {
    printf("%c\n", str[0]);
  } else {
		for (forward = 1; forward < len; forward++) {
			if (str[forward] < str[forward - 1]) {
				char temp = str[forward];
				int reverse = forward - 1;
				while (temp < str[reverse] && reverse >= 0) {
					str[reverse + 1] = str[reverse];
					str[reverse] = temp;
					reverse = reverse - 1;
				}
			}
			int i;
			for (i = 0; i < len; i++) {
					printf("%c",str[i]);
				}
			printf("\n");
		}
	}
}

void merge(char arr[100], int left, int middle, int right) {
	int i;
	int leftIndex = left;
	int rightIndex = middle + 1;
	char newArr[100];
  if (strlen(arr) <= 1) {
    return;
  }
	for(i = left; i <= right; i++) {
		if (arr[leftIndex] < arr[rightIndex] && leftIndex <= middle && rightIndex <= right) {
			newArr[i] = arr[leftIndex];
      leftIndex++;
		} else if (rightIndex <= right && leftIndex <= middle) {
			newArr[i] = arr[rightIndex];
      rightIndex++;
		} else {
      if (leftIndex > middle) {
        newArr[i] = arr[rightIndex];
        rightIndex++;
      } else {
        newArr[i] = arr[leftIndex];
        leftIndex++;
      }
    }
	}
	for(i = left; i <= right; i++) {
    printf("%c", newArr[i]);
		arr[i] = newArr[i];
	}
  printf("\n");
}

void mergeSort(char arr[100], int left, int right) {
	int middle = left + (right - left) / 2;
	if (right > left){
		mergeSort(arr, left, middle);
		mergeSort(arr, middle + 1, right);
    merge(arr, left, middle, right);
	}
}

void mergeInit() {
  char str[100];
  int success = scanf("%s", &str);
  int len = strlen(str);
  if (len == 1) {
    printf("%c\n", str[0]);
  } else if (len == 0) {
    return;
  }
	mergeSort(str, 0, len - 1);
}

void printAdjMap(char arr[27][27]) {
  int first, second;
  char i;
  printf("  ");
  for (i = 'A'; i <= 'Z'; i++) {
    printf("%c ", i);
  }
  printf("\n");
  i = 'A';
  for (first = 0; first < 27; first++) { 
    printf("%c ", i);
    i++;
    for (second = 0; second < 27; second++) {
      printf("%d ", arr[first][second]);
    }
    printf("\n");
  }
}

void BFSsearch(char adjmap[27][27], char currNode) {
  int row, col;
  char next[27] = {0};
  //Check if vertexes exists in adjmap
  for (col = 0; col < 27; col++) {
    //Vertex exists
    if (adjmap[currNode][col]) {
      next[col] = 1;
      printf("%c", col + 65);
      //Set node to visited
      for (row = 0; row < 27; row++) {
        adjmap[row][col] = 0;
      }
    }
  }
  for (row = 0; row < 27; row++) {
    if (next[row]) {
      BFSsearch(adjmap, row);
    }
  }
}

void BFS() {
  int startSet = 0;
  char start;
  // Basic adjmap build
  char adjmap[27][27] = {0};
  char str[100];
  int first, second;

  while (scanf("%s", &str) != EOF) {
    int len = strlen(str);
    if (!startSet) {
      start = str[0];
      startSet = 1;
    }
    for (first = 2; first < len; first++) {
      adjmap[str[0] - 65][str[first] - 65] = 1;
    }
  }
  printf("%c", start);
  char row;
  for (row = 0; row < 27; row++) {
    adjmap[row][start - 65] = 0;
  }
  BFSsearch(adjmap, start - 65);
  printf("\n");
  // end
}

void DFSsearch(char adjmap[27][27], char currNode) {
  char row, col;
  char next[27] = {0};
  //Check if vertexes exists in adjmap
  for (col = 0; col < 27; col++) {
    //Vertex exists
    if (adjmap[currNode][col]) {
      next[col] = 1;
      //Set node to visited
      for (row = 0; row < 27; row++) {
        adjmap[row][col] = 0;
      }
      printf("%c", col + 65);
      DFSsearch(adjmap, col);
    }
  }
}

void DFS() {
  int startSet = 0;
  char start;
  // Basic adjmap build
  char adjmap[27][27] = {0};
  char str[100];
  int first, second;

  while (scanf("%s", &str) != EOF) {
    int len = strlen(str);
    if (!startSet) {
      start = str[0];
      startSet = 1;
    }
    for (first = 2; first < len; first++) {
      adjmap[str[0] - 65][str[first] - 65] = 1;
    }
  }
  char row;
  for (row = 0; row < 27; row++) {
    adjmap[row][start - 65] = 0;
  }
  printf("%c", start);
  DFSsearch(adjmap, start - 65);
  printf("\n");
  // end
}

void dijkstra() {
  char adjmap[26][26] = {0};
  char str[100];
  char row, col;
  char visited[26] = {-1};
  char exists[26] = {-1};
  while (scanf("%s", &str) != EOF) {
    char from, to;
    int cost = 0;
    int len = strlen(str);
    from = str[0] - 65;
    to = str[2] - 65;
    // char to int and cost calculation
    int i = 0;
    for (i = 4; i < len; i++) {
        cost = 10 * cost + str[i] - 48;
    }
    adjmap[from][to] = cost;

    // Setting unvisited nodes
    visited[from] = 0;
    exists[from] = 

  }
  printAdjMap(adjmap);
  for ()
}


int main() {
  dijkstra();
  return (0);
}
