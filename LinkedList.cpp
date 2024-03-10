#include "LinkedList.h"
#include <iostream>

Node::Node() { this->previous = 0; }

Node::Node(char input[], Node *prev) {
  this->previous = prev;

  int i = 0;
  while (input[i] != '\0') {
    this->value[i] = input[i];
    i++;
  }
}

void Node::SetValue(char input[]) {
  int i = 0;
  while (input[i] != '\0') {
    this->value[i] = input[i];
    i++;
  }
}

void Node::SetPrevious(Node *prev) { this->previous = prev; }

char *Node::GetValue() {
  printf("\ngetting value from %p:\n", value);
  return value;
}
Node *Node::GetPrev() { return previous; }

Node::~Node() {}
