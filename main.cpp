#include "Stack.h"
#include <iostream>
#include <pthread.h>

int main(int argc, char **argv) {
  Stack *temp = new Stack;
  Stack stos = *temp;

  stos.print();
  std::cout << "\n\n";

  char opt = ' ';
  char buf[4];
  while (opt != 'q') {
    std::cin >> opt;
    switch (opt) {
    case 'p':
      std::cin >> buf;
      stos.push(buf);
      break;
    case 'i':
      stos.print();
      break;
    case 'r':
      stos.pop();
      break;
    }
  }
  return 0;
}
