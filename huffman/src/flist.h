#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef FLIST_H
#define FLIST_H

typedef struct flnode {
	unsigned int count;
	unsigned int data;
	struct flnode* next; //remove prev?
	struct flnode* prev;
} fListNode;

typedef struct flistheader {
	uint8_t unitsize;
	uint64_t count;
	fListNode* first;
	//add pointer to last?
} fList;

fList* newFreqList(uint8_t unitsize);
void fListInsert(fList* list, uint32_t data, uint32_t count);
void fListRemove(fList* list, fListNode* node);
fListNode* fListMin(fList* list);
fListNode* fListMax(fList* list);

void printall();


#endif