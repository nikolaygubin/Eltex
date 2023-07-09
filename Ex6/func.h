#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>

typedef struct {
  int (*sum)(int, int);
  int (*mult)(int, int);
  int (*sub)(int, int);
  int (*div)(int, int);
} operations;

int sum(int operand1, int operand2);
int mult(int operand1, int operand2);
int sub(int operand1, int operand2);
int div(int operand1, int operand2);
int process();

#endif  // FUNC_H