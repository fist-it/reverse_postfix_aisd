#include "Stack.h"
#include <iostream>

// CharStack {{{
CharStack::CharStack() {
  this->top = new CharNode;
  this->size = 0;
}

void CharStack::push(char input[]) {
  if (!size) {
    top->SetValue(input);
    size++;
  } else {
    CharNode *temp = this->top;
    top = new CharNode;
    top->SetPrevious(temp);
    top->SetValue(input);
    size++;
  }
}

void CharStack::pop() {
  if (size) {
    CharNode *temp = top->GetPrev();
    delete top;
    top = temp;
    size--;
  }
}

void CharStack::clear() {
  while (size > 0) {
    pop();
  }
}

void CharStack::print() {
  CharNode *current = this->top;
  while (current != 0) {
    std::cout << current->GetValue();
    current = current->GetPrev();
  }
}

void CharStack::free_stack() {
  int iter = size;
  for (int i = 0; i < iter; i++) {
    pop();
  }
}

CharStack::~CharStack() { free_stack(); }

// }}}

// IntStack {{{
IntStack::IntStack() {
  this->top = new IntNode;
  this->size = 0;
}

void IntStack::push(int input) {
  if (!size) {
    top->SetValue(input);
    size++;
  } else {
    IntNode *temp = this->top;
    top = new IntNode;
    top->SetPrevious(temp);
    top->SetValue(input);
    size++;
  }
}

void IntStack::pop() {
  if (size) {
    IntNode *temp = top->GetPrev();
    delete top;
    top = temp;
    size--;
  }
}

void IntStack::print() {
  std::cout << "\nsize: " << size << "\n";
  IntNode *current = this->top;
  for (int i = 0; i < size; i++) {
    std::cout << "\nnode " << size - i << ": " << current->GetValue() << "\n";
    current = current->GetPrev();
  }
}

void IntStack::free_stack() {
  int iter = size;
  for (int i = 0; i < iter; i++) {
    pop();
  }
}

IntStack::~IntStack() { free_stack(); }

// }}}
