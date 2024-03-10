#include "LinkedList.h"

class Stack {
private:
  Node *top;
  int size;

public:
  Stack();
  void push(char input[]);
  void pop();
  void print();
  void free_stack();
  ~Stack();
};
