#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

#include "func.h"

int process_plugin(operations *math) {
  int operand1, operand2, result;
  char symbol;

  printf(
      "Enabled operations:\n+ : sum\n- : sub\n* : mult\n/ : div\nEnter math "
      "expr format operand_1 operation operand 2 :\n");
  scanf("%d %c %d", &operand1, &symbol, &operand2);

  switch (symbol) {
    case '+':
      if (math->sum) result = math->sum(operand1, operand2);
      break;
    case '-':
      if (math->sub) result = math->sub(operand1, operand2);
      break;
    case '*':
      if (math->mult) result = math->mult(operand1, operand2);
      break;
    case '/':
      if (math->div) result = math->div(operand1, operand2);
      break;
    default:
      printf("Undefined operation!\n");
      return -1;
  }
  printf("Result:\n%d %c %d = %d\n", operand1, symbol, operand2, result);
  return 0;
}

void init_operations(operations *math) {
  math->sum = 0;
  math->mult = 0;
  math->sub = 0;
  math->div = 0;
}

int main() {
  operations math;
  init_operations(&math);

  void *my_lib = dlopen("./libmath.so", RTLD_LAZY);

  math.sum = dlsym(my_lib, "sum");
  math.mult = dlsym(my_lib, "mult");
  math.div = dlsym(my_lib, "div");
  math.sub = dlsym(my_lib, "sub");

  while (!process_plugin(&math)) {
  }

  dlclose(my_lib);

  return 0;
}