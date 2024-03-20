#include <stdio.h>
#include <stdlib.h>
#define CHARNODE_SIZE 4
#define BUFFSIZE 11
#define INIT_STR_SIZE 12

// priority enum {{{

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

int get_priority(const char op[]) {
  switch (op[0]) {
  case '+':
  case '-':
    return ADD;
  case '*':
  case '/':
    return MULTIPLY;
  case 'M':
    return MIN;
  case 'N':
    return N;
  case 'I':
    return IF;
  case 'P':
    return PAR;
  default:
    return -1;
  }
}

// }}}

// stack structs {{{
typedef struct charNode charNode;

struct charNode {
  char value[CHARNODE_SIZE];
  charNode *prev;
};

typedef struct {
  charNode *top;
  int size;
} charStack;
// }}}

// string {{{

typedef struct {
  char *data;
  int capacity;
  int length; // indexes from 0 to length-1
} string;

void appendchar(string *to, char input) {
  to->data[to->length] = input;
  to->length++;
}

void appendbytable(string *to, int input_size, char *alt_input) {
  int i = 0;
  if (to->length + input_size + 1 > to->capacity) { // 1 more space for space
    if (to->capacity) {
      //  to->data = (char *)realloc(to->data, to->capacity * 1.5);
      // realloc null handling
      void *temp = realloc(to->data, to->capacity * 1.5);
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

void freestr(string *str) { free(str->data); }

// }}}

// stack func {{{
void charpush(charStack *stack, const char *input) {
  if (!stack->size) {
    stack->top = (charNode *)malloc(sizeof(charNode));
    stack->top->prev = NULL;
  } else {
    charNode *temp = stack->top;
    stack->top = (charNode *)malloc(sizeof(charNode));
    stack->top->prev = temp;
  }
  int i = 0;
  while (input[i] != '\0') {
    stack->top->value[i] = input[i];
    i++;
  }
  stack->top->value[CHARNODE_SIZE - 1] = '\0';
  stack->size++;
}

void charpop(charStack *stack) {
  if (stack->size) {
    charNode *temp = stack->top->prev;
    free(stack->top);
    stack->top = temp;
    stack->size--;
  }
}

void printpop(charStack *stack, string *output) {
  appendbytable(output, 0, stack->top->value);
  charpop(stack);
}

void freecharstack(charStack *stack) {
  int iter = stack->size;
  for (int i = 0; i < iter; i++) {
    charpop(stack);
  }
}

void stackprint(charStack *stos, string *output) {
  charNode *current = stos->top;
  while (current != NULL) {
    // sprintf(output->data, current->value);
    appendbytable(output, 0, current->value);
    current = current->prev;
  }
}

void handleoperator(charStack *stos, int targetvalue, int is_rightassociative,
                    string *output, char *buff) {
  while (stos->size != 0 && (get_priority(stos->top->value) > targetvalue ||
                             (get_priority(stos->top->value) == targetvalue &&
                              !is_rightassociative))) {
    appendbytable(output, 0, stos->top->value);
    charpop(stos);
  }
  charpush(stos, buff);
}

void handle_negative(charStack *stos, string *output, char *input) {
  input[0] = '_';
  appendbytable(output, 0, input);
  char temp[2] = "N";
  charpush(stos, temp);
}

// }}}

int main(int argc, char **argv) {
  charStack stos;
  stos.size = 0;
  string postfix;
  postfix.length = 0;
  postfix.capacity = 0;
  int input_iter;
  scanf("%d", &input_iter);
  char buff[BUFFSIZE];
  for (int i = 0; i < input_iter; i++) {
    while (buff[0] != '.') {
      scanf("%s", buff);
      switch (buff[0]) {
      case '+':
        handleoperator(&stos, ADD, 0, &postfix, buff);
        break;
      case '*':
      case '/':
        handleoperator(&stos, MULTIPLY, 0, &postfix, buff);
        break;
      case 'M':
        if (buff[1] == 'I') {
          // TODO
          // handlemin(&stos, &postfix);
        } else {
          // TODO
          // handlemax(&stos, &postfix);
        }
        break;
      case '-':
        if (buff[1] == '\0') {
          handleoperator(&stos, ADD, 0, &postfix, buff);
        } else {
          handle_negative(&stos, &postfix, buff);
        }
        break;
      case '.':
        break;
      default:
        appendbytable(&postfix, 0, buff);
        break;
      }
    }
    stackprint(&stos, &postfix);
    printf("\n");
  }
  freecharstack(&stos);
  printf("%s", postfix.data);
  freestr(&postfix);
}
