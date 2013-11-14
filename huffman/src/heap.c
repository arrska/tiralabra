#include <stdlib.h>
#include <inttypes.h>

#include "heap.h"

heap* newHeap(uint32_t size) {
	heap* h = malloc(sizeof(heap));
	h->blocksize = 0;
	h->count = 0;
	h->size = size;
	h->nodes = malloc(sizeof(heapNode*)*size);
	
	return h;
}
heapNode* newHeapNode(uint32_t data, uint32_t value, uint8_t internal) {
	heapNode* newnode = malloc(sizeof(heapNode));
	
	newnode->data = data;
	newnode->value = value;
	newnode->index = -1;
	newnode->internal = internal;
	
	return newnode;
}

void heapInsert(heap* h, heapNode* nn) {
	if (h==NULL || nn==NULL) return;
	
	uint32_t i = h->count;
	h->count++;
	h->nodes[i] = nn;
	nn->index = i;
	
	if (i==0) return;
	
	heapNode* smaller;
	heapNode* node;
	heapNode* left;
	heapNode* right;
	node = parentNode(h, i);
	i = node->index;
	
	while (1) {
		left = leftChild(h, i);
		right = rightChild(h, i);
		smaller = NULL;
		
		if (left != NULL && left->value < node->value) {
			smaller = left;
		} else if (right != NULL && right->value < node->value) {
			smaller = right;
		} else {
			break;
		}
		swap(h, smaller, node);
		if (node->index <3) break;
		node = parentNode(h, smaller->index);
		i = node->index;
	}
}

heapNode* heapMin(heap* h) {
	return h->nodes[0];
}

heapNode* heapDeleteMin(heap* h) {
	if (h == NULL || h->count == 0) return NULL;
	
	heapNode* min = h->nodes[0];
	heapNode* last = h->nodes[h->count - 1];
	
	swap(h, min, last);
	//printf("min: %c ", min->data);
	h->count--;
	heapify(h, 0);
	
	return min;
}

void heapify(heap* h, uint32_t i) {
	heapNode* node;
	heapNode* left;
	heapNode* right;
	heapNode* smaller;
	
	node = h->nodes[i];
	
	//printf("size : %2d value: %d path: ", h->count, node->value);
	while (1) {
		//printf(" %d", node->index);
		left = leftChild(h, node->index);
		right = rightChild(h, node->index);
		
		if (right == NULL || left->value <= right->value) {
			smaller = left;
		} else if (left == NULL || right->value < left->value) {
			smaller = right;
		}
		
		if (smaller == NULL || smaller->value > node->value) break;
		
		swap(h, node, smaller);
	}
	
	//printf("\n");
	
	
	/*heapNode* parent;
	heapNode* node;
	
	node = h->nodes[i];
	parent = parentNode(h, i);
	while (parent->value > node->value) {
		swap(h, parent, node);
		parent = parentNode(h, node->index);
	}*/
}

/*
void heapify(heap* h, uint32_t i) {
	heapNode* smallest;
	heapNode* node;
	heapNode* left;
	heapNode* right;
	
	while (i >= 0) {
		node = h->nodes[i]
		left = leftChild(h, i);
		right = rightChild(h, i);
		smallest = NULL;
		
		if (left->value < node->value) {
			smallest = left;
		} else if (right->value < node->value) {
			smallest = right;
		} else {
			break;
		}
		swap(h, smallest, node);
		i = parent(h, smallest->index)->index;
	}
}*/

void swap(heap* h, heapNode* n1, heapNode* n2) {
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

/*uint8_t log2i(uint32_t num) {
	uint8_t i = 0;
	while (num>0) {
		num =<< 1;
		i++;
	}
	
	return i;
}
*/
