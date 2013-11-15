#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
#include "huff.h"


heap* loadheap(FILE* file, int blockSize) {
	if (file == NULL)
		return NULL;
	
	if (blockSize <= 0 || blockSize > sizeof(uint32_t))
		return NULL;
	
	uint32_t elems = 1 << blockSize*8;
	uint32_t* bytes = calloc(elems, sizeof(uint32_t));
	uint32_t* c = calloc(blockSize, sizeof(uint8_t));
	
	int ret = blockSize;
	
	bytes[*c]--;
	while (ret == blockSize) {
		bytes[*c]++;
		*c = 0;
		ret = fread(c, 1, blockSize, file);
	}
	//do something with last block if not full
	printf("last read: %d bytes, data: 0x%04x\n", ret, *c); 
	
	heap* h = newHeap(elems);
	
	for (int i = 0;i<elems;i++) {
		if (bytes[i] > 0) {
			heapInsert(h, newHeapNode(i, bytes[i]));
		}
	}
	/*
	for (int i = 0; i < h->count; i++) {
		printf("%2d val: %d data: 0x%02x (%c)\n", i, h->nodes[i]->value, h->nodes[i]->data, h->nodes[i]->data);
	}*/
	
	return h;
}

void emptyHeap(heap* h) {
	heapNode* n;
	
	while ((n=heapDeleteMin(h))) {
		printf("val: %2d   data: 0x%04x (%s)\n", n->value, n->data, (char*)&n->data);
		free(n);
	}
}

int main() {
    FILE* f = fopen("testdata", "r");
    int s = 1;
    heap* h = loadheap(f, sizeof(uint8_t)*s);
    huff(h);
	//emptyHeap(h);

    return 0;
}
