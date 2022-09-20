
#include <stdint.h>
#include <stdio.h>

#include "huffman.h"
#include "io.h"
#include "pq.h"
#include "stack.h"

// takes in histogram of uint64_t's of size ALPHABET
// constructs Huffman tree
// returns root node
Node *build_tree(uint64_t hist[static ALPHABET]) {
  PriorityQueue *pq = pq_create(ALPHABET);
  for (uint32_t i = 0; i < ALPHABET; i += 1) { // adding nodes to pq
    if (hist[i] > 0) {
      Node *n = node_create(i, hist[i]);
      enqueue(pq, n);
    }
  }
  while (pq_size(pq) > 1) { // build tree from pq
    Node *left = NULL;
    Node *right = NULL;
    dequeue(pq, &left);
    dequeue(pq, &right);
    Node *parent = node_join(left, right);
    enqueue(pq, parent);
  }
  Node *root = NULL;
  dequeue(pq, &root);
  pq_delete(&pq);
  return root;
}

// takes in Node root and Code table of size ALPHABET
// builds code for each symbol in Huffman tree and copies it to code table
void build_codes(Node *root, Code table[static ALPHABET]) {
  static Code c = {0};
  if (!root) {
    return;
  }
  if (!root->left && !root->right) { // leaf node
    table[root->symbol] = c;
  } else { // internal node
    uint8_t pop = 0;
    code_push_bit(&c, 0);
    build_codes(root->left, table);
    code_pop_bit(&c, &pop);
    code_push_bit(&c, 1);
    build_codes(root->right, table);
    code_pop_bit(&c, &pop);
  }
}

// takes in outfile file descriptor, pointer to a Node
// writes Huffman tree to outfile using a postorder traversal,
// L representing a leaf and I representing an interior node
void dump_tree(int outfile, Node *root) {
  if (root) {
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);
    if (!root->left && !root->right) { // leaf node
      uint8_t buff[1] = {'L'};
      write_bytes(outfile, buff, 1);
      uint8_t symbol = {root->symbol};
      write_bytes(outfile, &symbol, 1);
    } else { // interior node
      uint8_t symbol = {'I'};
      write_bytes(outfile, &symbol, 1);
    }
  }
}

// takes in int nbytes, tree dump of nbytes size
// reconstruct Huffman tree using a stack, given the tree dump
// returns root node
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  Stack *s = stack_create(nbytes);
  for (uint32_t i = 0; i < nbytes; i += 1) {
    if (tree[i] == 'L') { // leaf nodes
      Node *n = node_create(tree[i + 1], 1);
      stack_push(s, n);
      i += 1;                    // skip symbol after L
    } else if (tree[i] == 'I') { // interior nodes
      Node *left = NULL;
      Node *right = NULL;
      stack_pop(s, &right);
      stack_pop(s, &left);
      Node *parent = node_join(left, right);
      stack_push(s, parent);
    }
  }
  Node *root = NULL;
  stack_pop(s, &root);
  stack_delete(&s);
  return root;
}

// takes in Node
// deconstructs Huffman tree using a postorder traversal
void delete_tree(Node **root) {
  if (*root) {
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
    node_delete(root);
  }
}
