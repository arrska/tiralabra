#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef FLIST_H
#define	FLIST_H

typedef struct flnode {
	unsigned int count;
	unsigned int value;
	struct flnode* next;
	struct flnode* prev;
} fListNode;

typedef struct flistheader {
	uint8_t unitsize;
	uint64_t count;
	fListNode* first;
} fList;

fList* newFreqList(uint8_t unitsize);
void fListInsert(fList* list, uint32_t value, uint32_t count);
//void fListRemove();

//fListNode* fListMin();
//fListNode* fListMax();

void printall();


#endif