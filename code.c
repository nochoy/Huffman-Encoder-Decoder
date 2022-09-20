
#include "code.h"
#include "defines.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// takes in nothing
// Code constructor
// returns Code
Code code_init(void) {
  Code code;
  code.top = 0;
  for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
    code.bits[i] = 0;
  }
  return code;
}

// takes in code c
// return the size of the code
uint32_t code_size(Code *c) { return c->top; }

// takes in code c
// return boolean if code is empty
bool code_empty(Code *c) { return c->top == 0; }

// takes in code c
// return boolean if code is full
bool code_full(Code *c) { return c->top == MAX_CODE_SIZE * 8; }

// takes in code c and index i
// set bit i in code c to 1
// return boolean if successful
bool code_set_bit(Code *c, uint32_t i) {
  if (i > MAX_CODE_SIZE || i < 0) {
    return false;
  }
  c->bits[i / 8] |= (1 << (i % 8));
  return true;
}

// takes in code c and index i
// clear bit i in code c to 0
// return boolean if sucessful
bool code_clr_bit(Code *c, uint32_t i) {
  if (i > MAX_CODE_SIZE || i < 0) {
    return false;
  }
  c->bits[i / 8] &= ~(1 << (i % 8));
  return true;
}

// takes in code c and index i
// get bit i in code c
// return boolean if 0 or 1 or if out of bounds
bool code_get_bit(Code *c, uint32_t i) {
  if (i > MAX_CODE_SIZE || i < 0) {
    return false;
  }
  return (c->bits[i / 8] >> i % 8) & 0x1;
}

// takes in code c and bit to push
// push bit onto code c
// return boolean if successful
bool code_push_bit(Code *c, uint8_t bit) {
  if (!code_full(c)) {
    if (bit == 1) {
      if (!code_set_bit(c, c->top)) {
        return false;
      }
    } else {
      if (!code_clr_bit(c, c->top)) {
        return false;
      }
    }
    c->top += 1;
    return true;
  }
  return false;
}

// takes in code c and bit to return value
// pop bit off of code c
// return boolean if successful
bool code_pop_bit(Code *c, uint8_t *bit) {
  if (!code_empty(c)) {
    c->top -= 1;
    *bit = (int)code_get_bit(c, c->top);
    return true;
  }
  return false;
}

// helper function for code_print()
// print bits in byte
void print_bits_in_byte(uint8_t byte) {
  for (int8_t i = 7; i >= 0; i -= 1) {
    uint8_t bit = (byte >> i) & ((uint8_t)1);
    printf("%" PRIu8, bit);
  }
  printf("\n");
}

// print code c to stdout
void code_print(Code *c) {
  for (uint32_t i = 0; i < c->top; i += 1) {
    print_bits_in_byte(c->bits[i]);
    if ((i + 1) % 4 == 0) {
      printf(" ");
    }
    if (i == MAX_CODE_SIZE - 1) {
      printf("\n");
    }
  }
}
