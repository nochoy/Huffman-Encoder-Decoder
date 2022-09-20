
#include "pq.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// defines priority queue struct, implemented using a min heap
struct PriorityQueue {
  uint32_t capacity;
  uint32_t head; // end of the priority queue
  Node **nodes;
};

// takes in capacity
// constructor for priority queue
// returns pq
PriorityQueue *pq_create(uint32_t capacity) {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (pq) {
    pq->capacity = capacity;
    pq->head = 0; // 0 based indexing
    pq->nodes = (Node **)malloc(capacity * sizeof(Node));
    if (!pq->nodes) {
      free(pq);
      pq = NULL;
    }
  }
  return pq;
}

// takes in pq pointer to pointer
// destructor for priority queue
void pq_delete(PriorityQueue **pq) {
  if (*pq && (*pq)->nodes) {
    free((*pq)->nodes);
    free(*pq);
    *pq = NULL;
  }
}

// takes in pq
// return boolean if priority queue is empty
bool pq_empty(PriorityQueue *pq) {
  if (pq->head == 0) {
    return true;
  }
  return false;
}

// taes in pq
// return boolean if priority queue is full
bool pq_full(PriorityQueue *pq) {
  if (pq->head == pq->capacity) {
    return true;
  }
  return false;
}

// takes in pq
// return size of prioirty queue
uint32_t pq_size(PriorityQueue *pq) { return pq->head; }

// takes in PriorityQueue pq and index
// get node at index in the priority queue
// return pointer to node at index
static Node *pq_get(PriorityQueue *pq, uint32_t index) {
  return pq->nodes[index];
}

// takes in PriorityQueue pq, index i and j
// swaps the contents of i and j
static void swap(PriorityQueue *pq, uint32_t i, uint32_t j) {
  Node *temp = pq->nodes[i];
  pq->nodes[i] = pq->nodes[j];
  pq->nodes[j] = temp;
}

// takes in index i
// returns the index of the left child in the priority queue
static uint32_t left(uint32_t i) { return i * 2 + 1; }

// takes in index i
// returns the index of the right child in the priority queue
static uint32_t right(uint32_t i) { return i * 2 + 2; }

// takes in PriorityQueue pq, index i
// constructs a min heap
static void heapify(PriorityQueue *pq, uint32_t i) {
  uint32_t smallest = i;
  if (pq->head == 1) { // heap of size 1
    return;
  } else {
    if (left(i) < pq->head &&
        pq_get(pq, left(i))->frequency < pq_get(pq, smallest)->frequency) {
      smallest = left(i);
    }
    if (right(i) < pq->head &&
        pq_get(pq, right(i))->frequency < pq_get(pq, smallest)->frequency) {
      smallest = right(i);
    }
  }
  if (smallest != i) { // one of the children is smaller
    swap(pq, i, smallest);
    heapify(pq, smallest);
  }
}

// takes in PriorityQueue pq, node n
// enqueues node n into priority queue
// returns boolean if successful
bool enqueue(PriorityQueue *pq, Node *n) {
  if (!pq_full(pq)) {
    pq->nodes[pq->head] = n;
    pq->head += 1;
    for (int i = pq_size(pq) / 2 - 1; i >= 0;
         i -= 1) { // fix heap starting from first nonleaf node
      heapify(pq, i);
    }
    return true;
  }
  return false;
}

// takes in PriorityQueue pq and a pointer to a pointer to a node
// dequeues node from priority queue and returns it through n
// returns boolean if successful
bool dequeue(PriorityQueue *pq, Node **n) {
  if (!pq_empty(pq)) {
    *n = pq_get(pq, 0);        // highest priority is the root
    swap(pq, 0, pq->head - 1); // swap node to delete and last element
    pq->head -= 1;             // delete last element
    for (int i = pq_size(pq) / 2 - 1; i >= 0;
         i -= 1) { // fix heap starting from first nonleaf node
      heapify(pq, 0);
    }
    return true;
  }
  return false;
}

// print priority queue
// takes in PriorityQueue pq
void pq_print(PriorityQueue *pq) {
  for (uint32_t i = 0; i < pq->head; i += 1) {
    printf("\t");
    node_print(pq->nodes[i]);
  }
}
