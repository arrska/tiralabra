#include <stdlib.h>
#include <inttypes.h>

#ifndef HEAP_H
#define HEAP_H


typedef struct heapheader {
	uint32_t count;
	uint32_t size;
	
	struct heapnode** nodes;
} heap;

typedef struct heapnode {
	uint32_t data;
	uint32_t value;
	uint32_t index;
	
	struct heapnode* left;
	struct heapnode* right;
} heapNode;

heapNode* rightChild(heap* h, uint32_t i);
heapNode* leftChild(heap* h, uint32_t i);
heapNode* parentNode(heap* h, uint32_t i);
int swap(heap* h, heapNode* n1, heapNode* n2);
int heapify(heap* h, uint32_t i);
heapNode* heapDeleteMin(heap* h);
heapNode* heapMin(heap* h);
int heapInsert(heap* h, heapNode* nn);
heapNode* newHeapNode(uint32_t data, uint32_t value);
heap* newHeap(uint32_t size);
#endif
