#include "func.h"

int process() {
  int operand1, operand2, result;
  char symbol;

  printf(
      "Enabled operations:\n+ : sum\n- : sub\n* : mult\n/ : div\nEnter math "
      "expr format operand_1 operation operand 2 :\n");
  scanf("%d %c %d", &operand1, &symbol, &operand2);

  switch (symbol) {
    case '+':
      result = sum(operand1, operand2);
      break;
    case '-':
      result = sub(operand1, operand2);
      break;
    case '*':
      result = mult(operand1, operand2);
      break;
    case '/':
      result = div(operand1, operand2);
      break;
    default:
      printf("Undefined operation!\n");
      return -1;
  }
  printf("Result:\n%d %c %d = %d\n", operand1, symbol, operand2, result);
  return 0;
}