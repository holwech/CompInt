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

void BFS() {
  char adjmap[27][27];
  char str[100];
  int success = scanf("%s", &str);
  int len = strlen(str);

}

int main() {

  mergeInit();
  return (0);
}
