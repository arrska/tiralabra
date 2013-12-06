#include "heap.h"

heap* newHeap(uint32_t size) {
	heap* h = malloc(sizeof(heap));
	h->size = size;
	if (size > 0) {
		h->nodes = calloc(size, sizeof(heapNode*));
	} else {
		h->nodes = NULL;
	}
	h->count = 0;
	
	return h;
}
heapNode* newHeapNode(uint32_t data, uint32_t value) {
	heapNode* newnode = malloc(sizeof(heapNode));
	
	newnode->data = data;
	newnode->value = value;
	newnode->index = -1;
	
	newnode->left = NULL;
	newnode->right = NULL;
	
	return newnode;
}

int heapInsert(heap* h, heapNode* nn) {
	if (h==NULL || nn==NULL) return 1;
	if (h->count >= h->size) return 1;
	
	nn->index = h->count++;
	h->nodes[nn->index] = nn;
	
	heapNode* p;
	p = parentNode(h, nn->index);
	
	while (p != NULL && p->value > nn->value) {
		swap(h, nn, p);
		p = parentNode(h, nn->index);
	}
	
	return 0;
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

int heapify(heap* h, uint32_t i) {
	if (h == NULL) return 1;
	if (i >= h->count) return 1;
	
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
	
	return 0;
}

int swap(heap* h, heapNode* n1, heapNode* n2) {
	if (h==NULL || n1==NULL || n2==NULL) return 1;
	h->nodes[n1->index] = n2;
	h->nodes[n2->index] = n1;
	uint32_t index = n1->index;
	n1->index = n2->index;
	n2->index = index;
	
	return 0;
}

heapNode* parentNode(heap* h, uint32_t i) {
	if (h==NULL || i<=0 || i>=h->count) return NULL;
	return h->nodes[(i-1)/2];
}

heapNode* leftChild(heap* h, uint32_t i) {
	uint32_t c = (i+1)*2-1;
	if (h==NULL || c>=h->count) return NULL;
	return h->nodes[c];
}

heapNode* rightChild(heap* h, uint32_t i) {
	uint32_t c = (i+1)*2;
	if (h==NULL || c>=h->count) return NULL;
	return h->nodes[c];
}
