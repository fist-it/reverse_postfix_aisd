#include "Stack.h"

void init_charstack(stack *stos) { stos->size = 0; }
void init_intstack(countstack *stos) { stos->size = 0; }

void push(stack *stos, const char *input) {
  if (!stos->size) {
    stos->top = (charNode *)malloc(sizeof(charNode));
    stos->top->prev = NULL;
  } else {
    charNode *temp = stos->top;
    stos->top = (charNode *)malloc(sizeof(charNode));
    stos->top->prev = temp;
  }
  int i = 0;
  while (input[i] != '\0') {
    stos->top->value[i] = input[i];
    i++;
  }
  stos->top->value[i] = '\0';
  stos->size++;
}

void intpush(countstack *stos, const int input) {
  if (!stos->size) {
    stos->top = (intNode *)malloc(sizeof(intNode));
    stos->top->prev = NULL;
  } else {
    intNode *temp = stos->top;
    stos->top = (intNode *)malloc(sizeof(intNode));
    stos->top->prev = temp;
  }
  stos->top->value = input;
  stos->size++;
}

void intpop(countstack *stos) {
  if (stos->size) {
    intNode *temp = stos->top->prev;
    free(stos->top);
    stos->top = temp;
    stos->size--;
  }
}

void freeintstack(countstack *stos) {
  int iter = stos->size;
  for (int i = 0; i < iter; i++) {
    intpop(stos);
  }
}

void charpop(stack *stos) {
  if (stos->size) {
    charNode *temp = stos->top->prev;
    free(stos->top);
    stos->top = temp;
    stos->size--;
  }
}

void freecharstack(stack *stos) {
  int iter = stos->size;
  for (int i = 0; i < iter; i++) {
    charpop(stos);
  }
}

void stackprint(stack *stos, string *output) {
  charNode *current = stos->top;
  while (current != NULL) {
    appendbytable(output, 0, current->value);
    current = current->prev;
  }
}

void intstackprint(countstack *stos) {
  intNode *current = stos->top;
  while (current != NULL) {
    printf("%d ", current->value);
    current = current->prev;
  }
}
