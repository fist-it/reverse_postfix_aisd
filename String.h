#pragma once
#include "QOL.h"
#include <stdlib.h>

typedef struct {
  char *data;
  int capacity;
  int length; // indexes from 0 to length-1
} string;

void init_string(string *str);
void appendchar(string *to, char input);
void appendbytable(string *to, int input_size, char *alt_input);
void freestr(string *str);
