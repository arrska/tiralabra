#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "flist.h"
#include "heap.h"




void huff(FILE* file, int blockSize) {
	if (file == NULL)
		return;
	
	if (blockSize <= 0 || blockSize > sizeof(int))
		return;
	
	uint32_t elems = 1 << blockSize*8;
	uint32_t* bytes = calloc(elems, sizeof(uint32_t));
	uint8_t* c = calloc(1, sizeof(uint8_t));
	
	int ret;
	
	do {
		ret = fread(c, blockSize, 1, file);
		bytes[*c]++;
	} while (ret == 1);
	
	heap* h = newHeap(elems);
	
	for (int i = 0;i<elems;i++) {
		if (bytes[i] > 0) {
			//printf("inserting %d  0x%02x\n", bytes[i], i);
			heapInsert(h, newHeapNode(i, bytes[i], 0));
		}
	}
	
	//printf("min %d  0x%02x\n", heapMin(h)->value, heapMin(h)->data);
	
	for (int i = 0; i < h->count; i++) {
		printf("%2d val: %d data: 0x%02x (%c)\n", i, h->nodes[i]->value, h->nodes[i]->data, h->nodes[i]->data);
	}
	
	
	heapNode* n;
	
	while (n=heapDeleteMin(h)) {
		printf("val: %2d   data: 0x%02x (%c)\n", n->value, n->data, n->data);
		
		free(n);
	}
	return;
	/*
	fListNode* n2;
	fListNode* p;
	
	while(list->count > 1) {
		n = fListRemove(list, fListMin(list));
		n2 = fListRemove(list, fListMin(list));
		
		p = newfListNode(-1, n->count+n2->count, 1);
		
		if (n->count > n2->count) {
			p->left = n2;
			p->right = n;
		} else {
			p->left = n;
			p->right = n2;
		}
		
		fListInsert(list, p);
	}
	
	for (int i = 0;i<elems;i++) {
		if (bitcount[i] > 0) {
			printf("data: %02x (%c)    bits:%2d    code:0x%04x\n", i, i, bitcount[i], codes[i]);
		
		}
	}*/
}


int main() {
    FILE* f = fopen("testdata", "r");
    
    //buildTree(f, sizeof(uint8_t));
    huff(f, sizeof(uint8_t));

    return 0;
}
