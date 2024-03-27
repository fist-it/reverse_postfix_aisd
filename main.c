#include <stdio.h>
#include <stdlib.h>

#define CHARNODE_SIZE 4
#define BUFFSIZE 11
#define INIT_STR_SIZE 12
#define MAX_TAB_SIZE 14

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
} stack;

typedef struct intNode intNode;

struct intNode {
  int value;
  intNode *prev;
};

typedef struct {
  intNode *top;
  int size;
} countstack;

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

// }}}

// stack func {{{
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

void handleoperator(stack *stos, int targetvalue, int is_rightassociative,
                    string *output, char *buff) {
  while (stos->size != 0 && (get_priority(stos->top->value) > targetvalue ||
                             (get_priority(stos->top->value) == targetvalue &&
                              !is_rightassociative))) {
    appendbytable(output, 0, stos->top->value);
    charpop(stos);
  }
  push(stos, buff);
}

// }}}

// QOL {{{

void appendmin(string *postfix, countstack *counter) {
  appendchar(postfix, 'M');
  appendchar(postfix, 'I');
  appendchar(postfix, 'N');
  char count[BUFFSIZE];
  sprintf(count, "%d", counter->top->value);
  appendbytable(postfix, 0, count);
  intpop(counter);
}

void appendmax(string *postfix, countstack *counter) {
  appendchar(postfix, 'M');
  appendchar(postfix, 'A');
  appendchar(postfix, 'X');
  char count[BUFFSIZE];
  sprintf(count, "%d", counter->top->value);
  appendbytable(postfix, 0, count);
  intpop(counter);
}

int is_digit(char input) {
  if (input - '0' >= 0 && input - '0' <= 9) {
    return 1;
  } else {
    return 0;
  }
}

// }}}

// {{{
int calculate(string *postfix) {
  countstack nums;
  nums.size = 0;

  int i = 0;
  int num, mresult;
  int mcount = 0;
  while (postfix->data[i] != '\0') {
    if (postfix->data[i] == ' ') {
      i++;
      continue;
    }

    if (is_digit(postfix->data[i])) {
      num = 0;
      while (is_digit(postfix->data[i])) {
        num = num * 10 + (int)(postfix->data[i] - '0');
        i++;
      }
      intpush(&nums, num);

    } else if (postfix->data[i] == '-' && is_digit(postfix->data[i + 1])) {
      num = 0;
      i++;
      while (is_digit(postfix->data[i])) {
        num = num * 10 + (int)(postfix->data[i] - '0');
        i++;
      }
      intpush(&nums, -1 * num);
    } else {
      if (postfix->data[i] == 'N') {
        printf("N ");
        intstackprint(&nums);
        printf("\n");
        nums.top->value *= -1;
        i++;
      } else if (postfix->data[i] == 'I') {
        printf("IF ");
        intstackprint(&nums);
        printf("\n");
        int c = nums.top->value;
        intpop(&nums);
        int b = nums.top->value;
        intpop(&nums);
        int a = nums.top->value;
        intpop(&nums);

        if (a > 0) {
          intpush(&nums, b);
        } else {
          intpush(&nums, c);
        }
        i++;
      } else {
          int result = -1;
          int val1 = 0;
          if(nums.size != 0){
            val1 = nums.top->value;
          }
          intpop(&nums);
        switch (postfix->data[i]) {
        case '+':
          printf("+ %d ", val1);
          intstackprint(&nums);
          printf("\n");
          result = nums.top->value + val1;
          intpop(&nums);
          intpush(&nums, result);
          break;
        case '-':
          printf("- %d ", val1);
          intstackprint(&nums);
          printf("\n");
          result = nums.top->value - val1;
          intpop(&nums);
          intpush(&nums, result);
          break;
        case '*':
          printf("* %d ", val1);
          intstackprint(&nums);
          printf("\n");
          result = nums.top->value * val1;
          intpop(&nums);
          intpush(&nums, result);
          break;
        case '/':

          printf("/ %d ", val1);
          intstackprint(&nums);
          printf("\n");
          if (val1 == 0) {
            printf("ERROR\n");
            return 1;
          }
          result = nums.top->value / val1;
          intpop(&nums);
          intpush(&nums, result);
          break;

        case 'M':
          i++;
          mresult = val1;
          switch (postfix->data[i]) {
          case 'I':
            i += 2;
            while (is_digit(postfix->data[i])) {
              mcount = mcount * 10 + (int)(postfix->data[i] - '0');
              i++;
            }
            printf("MIN%d %d ", mcount, val1);
            intstackprint(&nums);
            if (mcount > 1) {
              for (int j = 0; j < mcount - 1; j++) {
                if (nums.size != 0) {
                  if (mresult > nums.top->value) {
                    mresult = nums.top->value;
                  }
                  intpop(&nums);
                }
              }
            }
            break;
          case 'A':
            i += 2;
            while (is_digit(postfix->data[i])) {
              mcount = mcount * 10 + (int)(postfix->data[i] - '0');
              i++;
            }
            printf("MAX%d %d ", mcount, val1);
            intstackprint(&nums);
            if (mcount > 1) {
              for (int j = 0; j < mcount - 1; j++) {
                if (nums.size != 0) {
                  if (mresult < nums.top->value) {
                    mresult = nums.top->value;
                  }
                    intpop(&nums);
                }
              }
            }
            break;
          }
          intpush(&nums, mresult);
          mcount = 0;
          printf("\n");
        }
      }
    }
    i++;
  }
  if(nums.size != 0){
      printf("%d", nums.top->value);
  }
  freeintstack(&nums);
  return 0;
}

// }}}

int main() {
  stack stos;
  stos.size = 0;
  countstack counter;
  counter.size = 0;

  string postfix;
  postfix.length = 0;
  postfix.capacity = 0;
  int input_iter;
  scanf("%d", &input_iter);
  char buff[BUFFSIZE];
  buff[0] = ' ';
  for (int i = 0; i < input_iter; i++) {
    while (buff[0] != '.') {
      scanf("%10s", buff);
      switch (buff[0]) {
      case '+':
      case '-':
        handleoperator(&stos, ADD, 0, &postfix, buff);
        break;
      case '*':
      case '/':
        handleoperator(&stos, MULTIPLY, 0, &postfix, buff);
        break;
      case 'N':
        handleoperator(&stos, N, 1, &postfix, buff);
        break;
      case '(':
        push(&stos, buff);
        break;
      case ')':
        while (stos.top->value[0] != '(') {
          appendbytable(&postfix, 0, stos.top->value);
          charpop(&stos);
        }
        charpop(&stos);
        if (stos.size && stos.top->value[0] == 'M') {
          if (stos.top->value[1] == 'I') {
            appendmin(&postfix, &counter);
          } else {
            appendmax(&postfix, &counter);
          }
          charpop(&stos);
        } else if(stos.size && stos.top->value[0] == 'I'){
            intpop(&counter);
        }
        break;
      case 'M':
      case 'I':
        push(&stos, buff);
        intpush(&counter, 1); // only so that counting arguments works
        break;
      case ',':
        while (stos.top->value[0] != '(') {
          appendbytable(&postfix, 0, stos.top->value);
          charpop(&stos);
        }
        if(counter.size && stos.top->value[0]) {
            counter.top->value++;
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
    appendchar(&postfix, '\0');
    printf("\n\n%s\n", postfix.data);
    calculate(&postfix);
    printf("\n");

    freestr(&postfix);
    printf("\n");
    buff[0] = ' ';
    freecharstack(&stos);
    freeintstack(&counter);
  }
  return 0;
}
