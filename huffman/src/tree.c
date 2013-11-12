#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree.h"
#include "flist.h"


uint8_t least(int bytes[256]) {
	uint8_t l = 0;
	int count = bytes[0];
	
	for (int i = 1;i<256;i++)
	if (bytes[i] < count) {
		l = i;
		count = bytes[i];
	}
	
	return l;
}

uint8_t most(int bytes[256]) {
	uint8_t m = 0;
	int count = bytes[0];
	
	for (int i = 1;i<256;i++)
	if (bytes[i] > count) {
		m = i;
		count = bytes[i];
	}
	
	return m;
}



tree* buildTree(FILE* file, int blockSize) {
	if (file == NULL)
		return NULL;
	
	if (blockSize <= 0 || blockSize > sizeof(int))
		return NULL;
	
	uint32_t elems = 1 << blockSize*8;
	unsigned int* bytes = calloc(elems, sizeof(int));

	unsigned int* c = calloc(1, sizeof(int));
	int ret;
	
	do {
		ret = fread(c, blockSize, 1, file);
		//printf("%s", c);
		//printf("%x ", *c);
		bytes[*c]++;
	} while (ret == 1);
	
	fList* list = newFreqList(blockSize);
	for (int i = 0;i<elems;i++) {
		if (bytes[i] > 0)
			fListInsert(list, newfListNode(i, bytes[i], 0));
		//fListInsert(list, i, bytes[i]);
	}
	
	fListNode* n;
	n = fListMax(list);
	printf("most of this: 0x%x, this many: %d\n", n->data, n->count);
	n = fListMin(list);
	printf("least of this: 0x%x, this many: %d\n", n->data, n->count);
	
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
	/*printf(
		"R:0x%08x (%1d) (%3d)  L:0x%08x (%1d) (%3d)\n", 
		p->right->data, 
			(p->right->right == NULL && p->right->left == NULL), 
			p->right->count, 
		p->left->data, 
			(p->left->right == NULL && p->left->left == NULL), 
		p->left->count);*/
	}
	
	//printf("r:0x%x l:0x%x\n", list->first->right->data, list->first->left->data);
	
	//DFS
	uint32_t* codes = calloc(elems, sizeof(uint32_t));
	uint32_t* bitcount = calloc(elems, sizeof(uint32_t));
	dfsTree(codes, bitcount, list->first, 0, 0);
	
	for (int i = 0;i<elems;i++) {
		if (bitcount[i] > 0) {
			printf("data: %02x    bits:%2d    code:0x%04x\n", i, bitcount[i], codes[i]);
		}
	}
	
	/*while (n = fListMin(list)) {
		if (n->count > 0)
			printf("0x%02x  %d\n", n->data, n->count);
		fListRemove(list, n);
	}*/
	//printall(list);
	
	return NULL;
}