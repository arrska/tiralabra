#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "heap.h"

#ifndef HUFF_H
#define HUFF_H

void dfsTree(heapNode* n, uint32_t* codes, uint32_t* codelens, uint32_t code, uint8_t codelen);
uint32_t* huff(heap* h, uint32_t* codelens);
//heapNode* pickMin(heap* h1, heap* h2);
void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint32_t* codelens, uint8_t block);
void readData(FILE* compf);
#endif
