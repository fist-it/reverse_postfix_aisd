#pragma once
#include "Nodes.h"
#include "QOL.h"
#include "String.h"

typedef struct {
  charNode *top;
  int size;
} stack;

typedef struct {
  intNode *top;
  int size;
} countstack;

void init_charstack(stack *stos);
void init_intstack(countstack *stos);
void push(stack *stos, const char *input);
void intpush(countstack *stos, const int input);
void intpop(countstack *stos);
void freeintstack(countstack *stos);
void charpop(stack *stos);
void freecharstack(stack *stos);
void stackprint(stack *stos, string *output);
void intstackprint(countstack *stos);
