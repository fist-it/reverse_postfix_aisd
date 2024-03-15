#include "Stack.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 5
#define BUFFSIZE 10

// String {{{

typedef struct {
  char *data;
  int len;
  int capacity;
} string;

void initstring(string *str) {
  str->data = (char *)malloc(INIT_CAP * sizeof(char));
  if (str->data == NULL) {
    fprintf(stderr, "Initialising string failed; malloc fail");
  }
  str->len = 0;
  str->capacity = INIT_CAP;
}

void appendChar(string *str, char ch) {
  if (str->len + 1 >= str->capacity) {
    str->capacity += 4;
    str->data = (char *)realloc(str->data, str->capacity * sizeof(char));
    if (str->data == NULL) {
      fprintf(stderr, "Initialising string failed; malloc fail");
    }
  }
  str->data[str->len] = ch;
  str->len++;
}

void freestring(string *str) {
  free(str->data);
  str->data = NULL;
  str->len = 0;
  str->capacity = 0;
}

void printstr(string *str) {
  for (int i = 0; i < str->len; i++) {
    std::cout << str->data[i];
  }
}

// }}}
int main(int argc, char **argv) {
  CharStack stos;
  IntStack stosliczb;
  string postfix;
  initstring(&postfix);

  int input_iterations;
  std::cin >> input_iterations;
  int topweight = 0;
  char buff[BUFFSIZE];
  for (int i = 0; i < input_iterations; i++) {
    while (buff[0] != '.') {
      std::cin >> buff;
      switch (buff[0]) {
      case '(':
        // TODO:
        // handle parenthesis
        break;
      case ')':
        break;

      // MIN/MAX switch {{{
      case 'M':
        if (buff[1] == 'I') {
          // TODO:
          // handle MIN;
          break;
        } else {
          // TODO
          // handle MAX;
        }
        break;
        // }}}

      case 'I':
        // TODO
        // handle IF(a, b, c)
        // if a>0 its b, otherwise c;
        break;
      // weight 2: {{{
      case '*':
        if (topweight > 2) {
          stos.print();
          topweight = 0;
          stos.free_stack();
        } else {
          topweight = 2;
          stos.push(buff);
        }
        break;
      case '/':
        if (topweight > 2) {
          stos.print();
          topweight = 0;
          stos.free_stack();
        } else {
          topweight = 2;
          stos.push(buff);
        }
        break;
        // }}}
      // weight 1: {{{
      case '+':
        if (topweight > 1) {
          stos.print();
          topweight = 0;
          stos.free_stack();
        } else {
          topweight = 1;
          stos.push(buff);
        }
        break;
      case '-':
        if (topweight > 1) {
          stos.print();
          topweight = 0;
          stos.free_stack();
        } else {
          topweight = 1;
          stos.push(buff);
        }
        break;
      // }}}
      case '.':
        break;
      default:
        /*
        char iter = 0;
        while (buff[iter] != '\0') {
          appendChar(&postfix, buff[iter]);
          iter++;
        }
        */
        std::cout << buff;
        break;
      }
    }
    stos.print();
    stos.free_stack();
    buff[0] = ' ';
    std::cout << std::endl;
  }

  char chuj;
  scanf("%c", &chuj);
  return 0;
}
