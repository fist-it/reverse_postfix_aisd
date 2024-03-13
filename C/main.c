#include <stdlib.h>

#define CHARNODE_SIZE 4

typedef struct intNode intNode;
struct intNode {
  int value;
  intNode *prev;
};

typedef struct charNode charNode;
struct charNode {
  char *value;
  charNode *prev;
};

typedef struct {
  intNode *top;
  int size;
} intStack;

typedef struct {
  charNode *top;
  int size;
} charStack;

void intpush(intStack *stack, int input) {
  if (!stack->size) {
    stack->top = (intNode *)malloc(sizeof(intNode));
    stack->top->prev = NULL;
  } else {
    intNode *temp = stack->top;
    stack->top = (intNode *)malloc(sizeof(intNode));
    stack->top->prev = temp;
  }
  stack->top->value = input;
  stack->size++;
}

void charpush(charStack *stack, char input[]) {
  if (!stack->size) {
    stack->top = (charNode *)malloc(sizeof(charNode));
    stack->top->prev = NULL;
  } else {
    charNode *temp = stack->top;
    stack->top = (charNode *)malloc(sizeof(charNode));
    stack->top->prev = temp;
  }
  int i = 0;
  while (input[i] != '\n') {
    stack->top->value[i] = input[i];
  }
  stack->top->value[CHARNODE_SIZE] = '\n';
  stack->size++;
}

void intpop(intStack *stack) {
  if (stack->size) {
    intNode *temp = stack->top->prev;
    free(stack->top);
    stack->top = temp;
    stack->size--;
  }
}

void charpop(charStack *stack) {
  if (stack->size) {
    charNode *temp = stack->top->prev;
    free(stack->top);
    stack->top = temp;
    stack->size--;
  }
}

void freeintstack(intStack *stack) {
  int iter = stack->size;
  for (int i = 0; i < iter; i++) {
    intpop(stack);
  }
}

void freecharstack(charStack *stack) {
  int iter = stack->size;
  for (int i = 0; i < iter; i++) {
    charpop(stack);
  }
}

int main(int argc, char **argv) {
  charStack chars;
  intStack ints;

  chars.size = 0;
  ints.size = 0;
}
