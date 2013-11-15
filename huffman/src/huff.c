#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "huff.h"

void huff(heap* h) {
	heapNode* l;
	heapNode* r;
	heapNode* nn;
	
	while (h->count > 1) {
		l = heapDeleteMin(h);
		r = heapDeleteMin(h);
		
		nn = newHeapNode(-1, l->value + r->value);
		nn->left = l;
		nn->right = r;
		
		heapInsert(h, nn);
	}
	
	uint32_t* codes = calloc(h->size, sizeof(uint32_t));
	dfsTree(heapDeleteMin(h), codes, 0, 0);
	free(h);
	
	/*heap* h2 = newHeap(h->size*2);
	heapNode* n1;
	heapNode* n2;
	heapNode* n3;
	
	do {
		n1 = heapDeleteMin(pickMin(h1, h2));
		n2 = heapDeleteMin(pickMin(h1, h2));
		
	} while (h->count > 0 && h2->count > 0);*/
}

void dfsTree(heapNode* n, uint32_t* codes, uint32_t code, uint8_t codelen) {
	if (n->left == NULL && n->right == NULL) {
		codes[n->data] = code;
		printf("data: 0x%04x (%1$c), code: 0x%04x, codelen: %d\n", n->data, code, codelen);
		return;
	}
	dfsTree(n->right, codes, (code << 1) | 1, codelen+1);
	dfsTree(n->left, codes, (code << 1), codelen+1);
}
/*
heap* pickMin(heap* h1, heap* h2) {
	heapNode* n1 = heapMin(h1);
	heapNode* n2 = heapMin(h2);
	
	if (n2 == NULL || n1->value > n2->value) {
		return h1;
	} else {
		return h2;
	}
}
*/
