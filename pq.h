#pragma once

#include "node.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *pq_create(uint32_t capacity);

void pq_delete(PriorityQueue **pq);

bool pq_empty(PriorityQueue *pq);

bool pq_full(PriorityQueue *pq);

uint32_t pq_size(PriorityQueue *pq);

bool enqueue(PriorityQueue *pq, Node *n);

bool dequeue(PriorityQueue *pq, Node **n);

void pq_print(PriorityQueue *pq);
