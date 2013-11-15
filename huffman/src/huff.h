#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "heap.h"

#ifndef HUFF_H
#define HUFF_H

void dfsTree(heapNode* n, uint32_t* codes, uint32_t code, uint8_t codelen);
void huff(heap* h);
heapNode* pickMin(heap* h1, heap* h2);
#endif
