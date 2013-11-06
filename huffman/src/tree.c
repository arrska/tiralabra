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
	
	unsigned int* bytes = calloc(1 << blockSize*8, sizeof(int));

	unsigned int* c = calloc(1, sizeof(int));
	int ret;
	
	do {
		ret = fread(c, blockSize, 1, file);
		//printf("%s", c);
		//printf("%x ", *c);
		bytes[*c]++;
	} while (ret == 1);
	
	fList* list = newFreqList(blockSize);
	for (int i = 0;i<list->count;i++) {
		fListInsert(list, i, bytes[i]);
	}
	
	fListNode* n;
	n = fListMax(list);
	printf("most of this: 0x%x, this many: %d\n", n->data, n->count);
	n = fListMin(list);
	printf("least of this: 0x%x, this many: %d\n", n->data, n->count);
	
	while (n = fListMin(list)) {
		if (n->count > 0)
			printf("0x%02x  %d\n", n->data, n->count);
		fListRemove(list, n);
	}
	//printall(list);
	
	return NULL;
}