#include <stdio.h>
#include <stdlib.h>

int fib(int idx) {
  if (idx > 1)
    return fib(idx - 1) + fib(idx - 2);
  else
    return idx;
}

int binary_search(const int *arr, int num, int lb, int ub) {
  int mid = lb + (ub - lb) / 2;

  if (arr[mid] == num)
    return mid;
  else if (mid == lb && mid == ub)
    return -1;
  else if (num < arr[mid])
    return binary_search(arr, num, lb, mid);
  else
    return binary_search(arr, num, mid + 1, ub);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  printf("%s член прогрессии : %d\n", argv[1], fib(atoi(argv[1])));

  int arr[] = {10, 16, 224, 400, 5899, 5901, 6000, 12700};
  const int num = 12700;

  printf("%d найден в массиве arr под индексом %d\n", num, binary_search(arr, num, 0, 7));
}