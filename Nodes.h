#pragma once
#include "QOL.h"

typedef struct charNode charNode;

struct charNode {
  char value[CHARNODE_SIZE];
  charNode *prev;
};

typedef struct intNode intNode;

struct intNode {
  int value;
  intNode *prev;
};
