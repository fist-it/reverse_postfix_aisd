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
    if (to->length + input_size + 1 > to->capacity) { // 1 more space for space
        if (to->capacity) {
            //  to->data = (char *)realloc(to->data, to->capacity * 1.5);
            // realloc null handling
            void *temp = realloc(to->data, to->capacity * 1.5);
            if (temp != NULL) {
                to->data = (char *) temp;
            } else {
                free(to->data);
                fprintf(stderr, "FATAL: string realloc error; aborting");
                exit(2);
            }
            to->capacity *= 3;
            to->capacity /= 2;
        } else {
            to->data = (char *) malloc(sizeof(char) * INIT_STR_SIZE);
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
void push(stack *stack, const char *input) {
    if (!stack->size) {
        stack->top = (charNode *) malloc(sizeof(charNode));
        stack->top->prev = NULL;
    } else {
        charNode *temp = stack->top;
        stack->top = (charNode *) malloc(sizeof(charNode));
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

void intpush(countstack *stack, const int input) {
    if (!stack->size) {
        stack->top = (intNode *) malloc(sizeof(intNode));
        stack->top->prev = NULL;
    } else {
        intNode *temp = stack->top;
        stack->top = (intNode *) malloc(sizeof(intNode));
        stack->top->prev = temp;
    }
    stack->top->value = input;
    stack->size++;
}

void intpop(countstack *stack) {
    if (stack->size) {
        intNode *temp = stack->top->prev;
        free(stack->top);
        stack->top = temp;
        stack->size--;
    }
}

void freeintstack(countstack *stack) {
    int iter = stack->size;
    for (int i = 0; i < iter; i++) {
        intpop(stack);
    }
}

void charpop(stack *stack) {
    if (stack->size) {
        charNode *temp = stack->top->prev;
        free(stack->top);
        stack->top = temp;
        stack->size--;
    }
}

void freecharstack(stack *stack) {
    int iter = stack->size;
    for (int i = 0; i < iter; i++) {
        charpop(stack);
    }
}

void stackprint(stack *stos, string *output) {
    charNode *current = stos->top;
    while (current != NULL) {
        // sprintf(output->data, current->value);
        appendbytable(output, 0, current->value);
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
    for (int i = 0; i < input_iter; i++) {
        while (buff[0] != '.') {
            scanf("%s", buff);
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
                    if (stos.top->value[0] == 'M') {
                        if (stos.top->value[1] == 'I') {
                            appendmin(&postfix, &counter);
                        } else {
                            appendmax(&postfix, &counter);
                        }
                        charpop(&stos);
                    }
                    break;
                case 'M':
                    push(&stos, buff);
                    intpush(&counter, 1);
                    break;
                case 'I':
                    push(&stos, buff);
                    break;
                case ',':
                    while (stos.top->value[0] != '(') {
                        appendbytable(&postfix, 0, stos.top->value);
                        charpop(&stos);
                    }
                    counter.top->value++;
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
    freeintstack(&counter);
    printf("%s", postfix.data);
    freestr(&postfix);
}
