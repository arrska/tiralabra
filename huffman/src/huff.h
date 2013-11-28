#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "heap.h"

#ifndef HUFF_H
#define HUFF_H

//void dfsTree(heapNode* n, uint32_t* codes, uint8_t* codelens, uint32_t code, uint8_t codelen);
void huff(heap* h);
//uint32_t* huff(heap* h, uint8_t* codelens);
//heapNode* pickMin(heap* h1, heap* h2);
//void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens, uint8_t block);
void readData(FILE* compf);
heap* loadHeap(FILE* file, uint8_t blockSize);
void writeHeader(FILE* compf, heapNode* root, uint8_t block, uint32_t* codes, uint8_t* codelens);
void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens, uint8_t block);
void compress(FILE* origf, FILE* compf, heap* h, uint8_t block);
#endif
