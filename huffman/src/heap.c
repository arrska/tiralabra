#include "heap.h"

heap* newHeap(uint32_t size) {
	heap* h = malloc(sizeof(heap));
	h->size = size;
	h->nodes = calloc(size, sizeof(heapNode*));
	h->count = 0;
	
	return h;
}
heapNode* newHeapNode(uint32_t data, uint32_t value) {
	heapNode* newnode = malloc(sizeof(heapNode));
	
	newnode->data = data;
	newnode->value = value;
	newnode->index = -1;
	
	return newnode;
}

void heapInsert(heap* h, heapNode* nn) {
	if (h==NULL || nn==NULL) return;
	
	nn->index = h->count++;
	h->nodes[nn->index] = nn;
	
	heapNode* p;
	p = parentNode(h, nn->index);
	
	while (p != NULL && p->value > nn->value) {
		swap(h, nn, p);
		p = parentNode(h, nn->index);
	}
}

heapNode* heapMin(heap* h) {
	if (h == NULL || h->count == 0) return NULL;
	return h->nodes[0];
}

heapNode* heapDeleteMin(heap* h) {
	if (h == NULL || h->count == 0) return NULL;
	
	heapNode* min = h->nodes[0];
	heapNode* last = h->nodes[h->count - 1];
	
	swap(h, min, last);
	h->count--;
	heapify(h, 0);
	
	return min;
}

void heapify(heap* h, uint32_t i) {
	heapNode* n;
	heapNode* left;
	heapNode* right;
	heapNode* smaller = NULL;
	
	n = h->nodes[i];
	
	while ((left = leftChild(h, n->index))) {
		right = rightChild(h, n->index);
		
		if (right == NULL) {
			smaller = left;
		} else {
			smaller = left->value < right->value ? left : right;
		}
		
		if (smaller->value >= n->value) break;
		swap(h, n, smaller);
	}
}

void swap(heap* h, heapNode* n1, heapNode* n2) {
	if (h==NULL || n1==NULL || n2==NULL) return;
	h->nodes[n1->index] = n2;
	h->nodes[n2->index] = n1;
	uint32_t index = n1->index;
	n1->index = n2->index;
	n2->index = index;
}

heapNode* parentNode(heap* h, uint32_t i) {
	if (i<=0) return NULL;
	return h->nodes[(i-1)/2];
}

heapNode* leftChild(heap* h, uint32_t i) {
	uint32_t c = (i+1)*2-1;
	if (c >= h->count) return NULL;
	return h->nodes[c];
}

heapNode* rightChild(heap* h, uint32_t i) {
	uint32_t c = (i+1)*2;
	if (c >= h->count) return NULL;
	return h->nodes[c];
}

void emptyHeap(heap* h) {
	heapNode* n;
	
	while ((n=heapDeleteMin(h))) {
		printf("val: %2d   data: 0x%04x (%s)\n", n->value, n->data, (char*)&n->data);
		free(n);
	}
}
