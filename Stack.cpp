#include "Stack.h"
#include <iostream>

Stack::Stack() {
  this->top = new Node;
  this->size = 0;
}

void Stack::push(char input[]) {
  if (!size) {
    top->SetValue(input);
    size++;
  } else {
    Node *temp = this->top;
    top = new Node;
    top->SetPrevious(temp);
    top->SetValue(input);
    size++;
  }
}

void Stack::pop() {
  if (size) {
    Node *temp = top->GetPrev();
    delete top;
    top = temp;
    size--;
  }
}

void Stack::print() {
  std::cout << "\nsize: " << size << "\n";
  Node *current = this->top;
  for (int i = 0; i < size; i++) {
    std::cout << "\nnode " << size - i << ": " << current->GetValue() << "\n";
    current = current->GetPrev();
  }
}

void Stack::free_stack() {
  int iter = size;
  for (int i = 0; i < iter; i++) {
    pop();
  }
}

Stack::~Stack() { free_stack(); }
