#pragma once
#include <stdio.h>
#include <stdlib.h>

#define CHARNODE_SIZE 4
#define BUFFSIZE 11
#define INIT_STR_SIZE 12
#define MAX_TAB_SIZE 14

typedef enum {
  ADD = 1,
  SUBTRACT = 1,
  MULTIPLY = 2,
  DIVIDE = 2,
  IF = 3,
  MIN = 3,
  MAX = 3,
  N = 3,
  PAR = 4
} Priority;

int get_priority(const char op[]);
int is_digit(char input);
