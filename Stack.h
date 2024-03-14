#include "LinkedList.h"

class CharStack {
private:
  CharNode *top;
  int size;

public:
  CharStack();
  void push(char input[]);
  void pop();
  void clear();
  void print();
  void free_stack();
  ~CharStack();
};

class IntStack {
private:
  IntNode *top;
  int size;

public:
  IntStack();
  void push(int input);
  void pop();
  void print();
  void free_stack();
  ~IntStack();
};
