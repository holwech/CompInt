#include<stdio.h>
#include <string.h>
#include <math.h>

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

void printAdjMap(char arr[26][26]) {
  int first, second;
  char i;
  printf("  ");
  for (i = 'A'; i <= 'Z'; i++) {
    printf("%c ", i);
  }
  printf("\n");
  i = 'A';
  for (first = 0; first < 26; first++) { 
    printf("%c ", i);
    i++;
    for (second = 0; second < 26; second++) {
      printf("%d ", arr[first][second]);
    }
    printf("\n");
  }
}

void BFSsearch(char adjmap[26][26], char currNode) {
  int row, col;
  char next[27] = {0};
  //Check if vertexes exists in adjmap
  for (col = 0; col < 27; col++) {
    //Vertex exists
    if (adjmap[currNode][col]) {
      next[col] = 1;
      printf("%c", col + 65);
      //Set node to visited
      for (row = 0; row < 26; row++) {
        adjmap[row][col] = 0;
      }
    }
  }
  for (row = 0; row < 26; row++) {
    if (next[row]) {
      BFSsearch(adjmap, row);
    }
  }
}

void BFS() {
  int startSet = 0;
  char start;
  // Basic adjmap build
  char adjmap[26][26] = {0};
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
  for (row = 0; row < 26; row++) {
    adjmap[row][start - 65] = 0;
  }
  BFSsearch(adjmap, start - 65);
  printf("\n");
  // end
}

void DFSsearch(char adjmap[26][26], char currNode) {
  char row, col;
  char next[26] = {0};
  //Check if vertexes exists in adjmap
  for (col = 0; col < 26; col++) {
    //Vertex exists
    if (adjmap[currNode][col]) {
      next[col] = 1;
      //Set node to visited
      for (row = 0; row < 26; row++) {
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
  char adjmap[26][26] = {0};
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
  for (row = 0; row < 26; row++) {
    adjmap[row][start - 65] = 0;
  }
  printf("%c", start);
  DFSsearch(adjmap, start - 65);
  printf("\n");
  // end
}

double inf = 127;

void dijkstraSearch(char visited[26], char exists[26], char currNode, char adjmap[26][26], int costList[26], char goal, char parent[26]) {
  int vertex;
  visited[currNode] = 1;
  int minVertex = inf;
  char next = -1;
  for (vertex = 0; vertex < 26; vertex++) {
    // Update cost list
    if((costList[currNode] + adjmap[currNode][vertex]) < costList[vertex] && visited[vertex] == 0) {
      costList[vertex] = adjmap[currNode][vertex] + costList[currNode];
      parent[vertex] = currNode;
    }
  }
  // Find next vertex
  int cost;
  for (cost = 0; cost < 26; cost++) {
    for (vertex = 0; vertex < 26; vertex++) {
      if((costList[cost] + adjmap[cost][vertex]) < minVertex && visited[vertex] == 0) {
        next = vertex;
        minVertex = costList[cost] + adjmap[cost][vertex];
      }
    }
  }
  // If goal is reached
  if (next == -1) {
    return;
  } else {
    // Keep searching
    dijkstraSearch(visited, exists, next, adjmap, costList, goal, parent);
  }
}

int findChar(char find, char arr[26], int len) {
  int j;
  for (j = 0; j < len; j++) {
    if (arr[j] == find) {
      return j;
    }
  }
  return -1;
}

void printPath(char start, char goal, char parent[26], int costList[26]) {
  int next = goal;
  int i = 25;
  char charsToPrint[26] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  charsToPrint[i] = goal;
  i--;
  while (next != start) {
    charsToPrint[i] = parent[next];
    i--;
    next = parent[next];
  } 
  int j;
  for (j = i + 1; j < 26; j++) {
    printf("%c-", charsToPrint[j] + 65);
  }
  printf("%d\n", costList[goal]);
}

void dijkstra() {
  char adjmap[26][26] = {inf};
  int m, l;
  for (m = 0; m < 26; m++) {
    for (l = 0; l < 26; l++) {
      adjmap[m][l] = inf;
    }
  }
  char str[100];
  char row, col;
  char goal = -1;  
  char visited[26] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  char exists[26] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  char from, to;
  while (scanf("%s", &str) != EOF) {
    if (goal == -1) {
      goal = str[0] - 65;
    }
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
    exists[from] = 1; 
    adjmap[from][from] = 0;
  }
  // Cost to self is 0
  //  printAdjMap(adjmap);
  int i;
  for (i = 0; i < 26; i++) {
    if (exists[i] == 1) {
      char parent[26] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
      int costList[26] = {inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf,inf};
      costList[i] = 0;
      char tempVisited[26];
      memcpy(tempVisited, visited, 26);
      dijkstraSearch(tempVisited, exists, i, adjmap, costList, goal, parent);
      printPath(i, goal, parent, costList);
    }
  }
}



int main() {
  dijkstra();
  return (0);
}
