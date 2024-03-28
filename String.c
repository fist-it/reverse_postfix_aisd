#include "String.h"

void appendchar(string *to, char input) {
  to->data[to->length] = input;
  to->length++;
}

void appendbytable(string *to, int input_size, char *alt_input) {
  int i = 0;
  if (to->length + input_size + 1 > to->capacity ||
      (!input_size && to->length + MAX_TAB_SIZE + 1 >
                          to->capacity)) { // 1 more space for space
    if (to->capacity) {
      //  to->data = (char *)realloc(to->data, to->capacity * 1.5);
      // realloc null handling
      void *temp = realloc(to->data, to->capacity * 3 / 2);
      if (temp != NULL) {
        to->data = (char *)temp;
      } else {
        free(to->data);
        fprintf(stderr, "FATAL: string realloc error; aborting");
        exit(2);
      }
      to->capacity *= 3;
      to->capacity /= 2;
    } else {
      to->data = (char *)malloc(sizeof(char) * INIT_STR_SIZE);
      to->capacity = INIT_STR_SIZE;
    }
  }
  while (alt_input[i] != '\0') {
    if (alt_input[i] != '_') {
      appendchar(to, alt_input[i]);
    }
    i++;
  }
  appendchar(to, ' ');
}

void freestr(string *str) {
  str->capacity = 0;
  str->length = 0;
  free(str->data);
}
