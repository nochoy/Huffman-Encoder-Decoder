
#include "node.h"
#include "defines.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// takes in symbol and frequency
// constructor for node
// returns node
Node *node_create(uint8_t symbol, uint64_t frequency) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node) {
    node->left = NULL;
    node->right = NULL;
    node->symbol = symbol;
    node->frequency = frequency;
  }
  return node;
}

// takes in node double pointer
// destructor for node
void node_delete(Node **n) {
  if (*n) {
    free(*n);
    *n = NULL;
  }
}

// takes in left and right child nodes
// joins left and right child nodes to create a parent node
// returns parent node
Node *node_join(Node *left, Node *right) {
  Node *n = node_create('$', left->frequency + right->frequency);
  n->left = left;
  n->right = right;
  return n;
}

// takes in node n
// print node
void node_print(Node *n) {
  if (!n) {
    return;
  }
  printf("parent: %d - %" PRIu64 "\n", n->symbol, n->frequency);
  if (n->left != NULL) {
    printf("\t");
    node_print(n->left);
  }
  if (n->right != NULL) {
    printf("\t");
    node_print(n->right);
  }
}
