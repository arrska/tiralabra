#ifndef HEAP_H
#define HEAP_H

#include "flist.h"


typedef struct heapheader {
	uint8_t blocksize;
	uint32_t count;
	uint32_t size;
	
	struct heapnode** nodes;
} heap;

typedef struct heapnode {
	uint32_t value;
	uint32_t data;
	uint32_t index;
	
	uint8_t internal;
} heapNode;

heapNode* rightChild(heap* h, uint32_t i);
heapNode* leftChild(heap* h, uint32_t i);
heapNode* parentNode(heap* h, uint32_t i);
void swap(heap* h, heapNode* n1, heapNode* n2);
void heapify(heap* h, uint32_t i);
heapNode* heapDeleteMin(heap* h);
heapNode* heapMin(heap* h);
void heapInsert(heap* h, heapNode* nn);
heapNode* newHeapNode(uint32_t data, uint32_t value, uint8_t internal);
heap* newHeap(uint32_t size);
#endif
