#include "QOL.h"
#include "Stack.h"
#include "String.h"
#include <stdio.h>
#include <stdlib.h>

// rpn-related functions {{{
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

// single-case Mxx functions preventing space after Mxx in output (for argument
// counting)

void appendmin(string *postfix, countstack *counter) { // by default appending
                                                       // by table places a
                                                       // space after
  appendchar(postfix, 'M');
  appendchar(postfix, 'I');
  appendchar(postfix, 'N');
  char count[BUFFSIZE];
  sprintf(count, "%d", counter->top->value);
  appendbytable(postfix, 0, count);
  intpop(counter);
}

void appendmax(string *postfix, countstack *counter) { // by default appending
                                                       // by table places a
                                                       // space after

  appendchar(postfix, 'M');
  appendchar(postfix, 'A');
  appendchar(postfix, 'X');
  char count[BUFFSIZE];
  sprintf(count, "%d", counter->top->value);
  appendbytable(postfix, 0, count);
  intpop(counter);
}

void handle_function(stack *stos, countstack *counter, char *buff) {
  push(stos, buff);
  intpush(counter, 1);
}

void handle_period(stack *stos, string *postfix, countstack *counter) {
  while (stos->top->value[0] != '(') {
    appendbytable(postfix, 0, stos->top->value);
    charpop(stos);
  }
  if (counter->size && stos->top->value[0]) {
    counter->top->value++;
  }
}

void handle_close(stack *stos, string *postfix, countstack *counter) {
  while (stos->top->value[0] != '(') {
    appendbytable(postfix, 0, stos->top->value);
    charpop(stos);
  }
  charpop(stos);
  if (stos->size && stos->top->value[0] == 'M') {
    if (stos->top->value[1] == 'I') {
      appendmin(postfix, counter);
    } else {
      appendmax(postfix, counter);
    }
    charpop(stos);
  } else if (stos->size && stos->top->value[0] == 'I') {
    intpop(counter);
  }
}

// }}}

// simple operator calculations {{{

void calc_plus(countstack *const nums, const int *val1) {
  printf("+ %d ", *val1);
  intstackprint(nums);
  printf("\n");
  int result = nums->top->value + *val1;
  intpop(nums);
  intpush(nums, result);
}

void calc_minus(countstack *const nums, const int *val1) {
  printf("- %d ", *val1);
  intstackprint(nums);
  printf("\n");
  int result = nums->top->value - *val1;
  intpop(nums);
  intpush(nums, result);
}

void calc_multiply(countstack *const nums, const int *val1) {
  printf("* %d ", *val1);
  intstackprint(nums);
  printf("\n");
  int result = nums->top->value * *val1;
  intpop(nums);
  intpush(nums, result);
}

int calc_division(countstack *const nums, const int *val1) {
  printf("/ %d ", *val1);
  intstackprint(nums);
  printf("\n");
  if (*val1 == 0) {
    printf("ERROR\n");
    return 1;
  }
  int result = nums->top->value / *val1;
  intpop(nums);
  intpush(nums, result);
  return 0;
}

void calc_number(countstack *const nums, const string *postfix,
                 int *const iterator) {
  int result = 0;
  while (is_digit(postfix->data[*iterator])) {
    result = result * 10 + (int)(postfix->data[*iterator] - '0');
    (*iterator)++;
  }
  intpush(nums, result);
}

void calc_if(countstack *const nums) {
  printf("IF ");
  intstackprint(nums);
  printf("\n");
  int c = nums->top->value;
  intpop(nums);
  int b = nums->top->value;
  intpop(nums);
  int a = nums->top->value;
  intpop(nums);

  if (a > 0) {
    intpush(nums, b);
  } else {
    intpush(nums, c);
  }
}
void calc_n(countstack *const nums) {
  printf("N ");
  intstackprint(nums);
  printf("\n");
  nums->top->value *= -1;
}

// }}}

// more complex calculations (M functions) {{{

void handle_M(string *postfix, int *iter, int *val1, countstack *nums) {
  (*iter)++;
  int result = *val1;
  int argcount = 0;
  switch (postfix->data[*iter]) {
  case 'I':
    (*iter) += 2;
    while (is_digit(postfix->data[*iter])) {
      argcount = argcount * 10 + (int)(postfix->data[*iter] - '0');
      (*iter)++;
    }
    printf("MIN%d %d ", argcount, *val1);
    intstackprint(nums);
    if (argcount > 1) {
      for (int j = 0; j < argcount - 1; j++) {
        if (nums->size != 0) {
          if (result > nums->top->value) {
            result = nums->top->value;
          }
          intpop(nums);
        }
      }
    }
    break;
  case 'A':
    (*iter) += 2;
    while (is_digit(postfix->data[*iter])) {
      argcount = argcount * 10 + (int)(postfix->data[*iter] - '0');
      (*iter)++;
    }
    printf("MAX%d %d ", argcount, *val1);
    intstackprint(nums);
    if (argcount > 1) {
      for (int j = 0; j < argcount - 1; j++) {
        if (nums->size != 0) {
          if (result < nums->top->value) {
            result = nums->top->value;
          }
          intpop(nums);
        }
      }
    }
    break;
  }
  intpush(nums, result);
  printf("\n");
}

// }}}

// calculating {{{
int calculate(string *postfix) {
  countstack nums;
  init_intstack(&nums);

  int i = 0;
  while (postfix->data[i] != '\0') {
    // ignore spaces between tokens
    if (postfix->data[i] == ' ') {
      i++;
      continue;
    }

    if (is_digit(postfix->data[i])) {
      calc_number(&nums, postfix, &i);
    } else {
      if (postfix->data[i] == 'N') {
        calc_n(&nums);
        i++;
      } else if (postfix->data[i] == 'I') {
        calc_if(&nums);
        i++;
      } else {
        int val1 = 0;
        // prevent bad access
        if (nums.size != 0) {
          val1 = nums.top->value;
        }
        intpop(&nums);
        switch (postfix->data[i]) {
        case '+':
          calc_plus(&nums, &val1);
          break;
        case '-':
          calc_minus(&nums, &val1);
          break;
        case '*':
          calc_multiply(&nums, &val1);
          break;
        case '/':
          // if division fails:
          if (calc_division(&nums, &val1)) {
            return 1;
          }
          break;

        case 'M':
          handle_M(postfix, &i, &val1, &nums);
        }
      }
    }
    i++;
  }
  if (nums.size != 0) {
    printf("%d", nums.top->value);
  }
  freeintstack(&nums);
  return 0;
}

// }}}

void end_iter_routine(stack *stos, string *postfix, countstack *counter) {
  stackprint(stos, postfix);
  appendchar(postfix, '\0');
  printf("\n\n%s\n", postfix->data);
  calculate(postfix);
  printf("\n");

  freestr(postfix);
  printf("\n");
  freecharstack(stos);
  freeintstack(counter);
}

// main {{{

int main() {
  stack stos;
  init_charstack(&stos);
  countstack counter;
  init_intstack(&counter);
  string postfix;
  init_string(&postfix);

  int input_iter;
  scanf("%d", &input_iter);
  char buff[BUFFSIZE];
  // initialising first buff char, so that it wont check uninitialised memory on
  // loop start
  for (int i = 0; i < input_iter; i++) {
    buff[0] = ' ';
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
        handle_close(&stos, &postfix, &counter);
        break;
      case 'M':
      case 'I':
        handle_function(&stos, &counter, buff);
        break;
      case ',':
        handle_period(&stos, &postfix, &counter);
        break;
      case '.':
        break;
      default:
        appendbytable(&postfix, 0, buff);
        break;
      }
    }
    end_iter_routine(&stos, &postfix, &counter);
  }
  return 0;
}

// }}}
