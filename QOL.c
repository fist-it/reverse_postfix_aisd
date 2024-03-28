#include "QOL.h"

int get_priority(const char op[]) {
  switch (op[0]) {
  case '+':
  case '-':
    return ADD;
  case '*':
  case '/':
    return MULTIPLY;
  case 'M':
    return MIN;
  case 'N':
    return N;
  case 'I':
    return IF;
  case 'P':
    return PAR;
  default:
    return -1;
  }
}

int is_digit(char input) {
  if (input - '0' >= 0 && input - '0' <= 9) {
    return 1;
  } else {
    return 0;
  }
}
