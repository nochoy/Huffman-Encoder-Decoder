
#include "stack.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// defines stack struct
struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

// takes in capacity of stack
// creates stack
// returns stack
Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (s) {
    s->top = 0;
    s->capacity = capacity;
    s->items = calloc(capacity, sizeof(Node));
    if (!s->items) {
      free(s);
      s = NULL;
    }
  }
  return s;
}

// takes in double pointer to stack
// deletes stack
void stack_delete(Stack **s) {
  if (*s && (*s)->items) {
    free((*s)->items);
    free(*s);
    *s = NULL;
  }
  return;
}

// takes in stack
// returns size of stack
uint32_t stack_size(Stack *s) { return (s->top); }

// takes in stack
// returns boolean if stack is empty
bool stack_empty(Stack *s) {
  if (s->top == 0) {
    return true;
  }
  return false;
}

// takes in stack
// returns boolean if stack is full
bool stack_full(Stack *s) {
  if (s->top == s->capacity) {
    return true;
  }
  return false;
}

// takes in stack, node n
// pushes node n on to stack
// returns boolean if successful
bool stack_push(Stack *s, Node *n) {
  if (!stack_full(s)) {
    s->items[s->top] = n;
    s->top += 1;
    return true;
  }
  return false;
}
// takes in stack, pointer to node n
// pops top item off stack and returns node through pointer n
// returns boolean if successful
bool stack_pop(Stack *s, Node **n) {
  if (!stack_empty(s)) {
    s->top -= 1;
    *n = s->items[s->top];
    return true;
  }
  return false;
}

// print the stack in outfile
// takes in outfile, array of vertices names
void stack_print(Stack *s) {
  for (uint32_t i = 0; i < s->top; i += 1) {
    node_print(s->items[i]);
  }
}
