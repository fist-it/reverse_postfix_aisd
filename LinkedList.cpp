#include "LinkedList.h"
#include <iostream>

/// CharNode {{{
CharNode::CharNode() { this->previous = 0; }

CharNode::CharNode(char input[], CharNode *prev) {
  this->previous = prev;

  int i = 0;
  while (input[i] != '\0') {
    this->value[i] = input[i];
    i++;
  }
}

void CharNode::SetValue(char input[]) {
  int i = 0;
  while (input[i] != '\0') {
    this->value[i] = input[i];
    i++;
  }
}

void CharNode::SetPrevious(CharNode *prev) { this->previous = prev; }

char *CharNode::GetValue() {
  printf("\ngetting value from %p:\n", value);
  return value;
}
CharNode *CharNode::GetPrev() { return previous; }

CharNode::~CharNode() {}

// }}}

// IntNode {{{
IntNode::IntNode() { this->previous = 0; }

IntNode::IntNode(int input, IntNode *prev) {
  this->previous = prev;
  this->value = input;
}

void IntNode::SetValue(int input) { this->value = input; }

void IntNode::SetPrevious(IntNode *prev) { this->previous = prev; }

int IntNode::GetValue() {
  printf("\ngetting value from %d:\n", value);
  return value;
}

IntNode *IntNode::GetPrev() { return previous; }

IntNode::~IntNode() {}

// }}}
