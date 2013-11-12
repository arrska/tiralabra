#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef FLIST_H
#define FLIST_H

typedef struct flnode {
	unsigned int count;
	
	unsigned int data;
	
	uint8_t internal;
	
	struct flnode* next;
	//remove prev?
	struct flnode* prev;
	
	struct flnode* left;
	struct flnode* right;
} fListNode;

typedef struct flistheader {
	uint8_t unitsize;
	uint64_t count;
	fListNode* first;
	//add pointer to last?
} fList;

fList* newFreqList(uint8_t unitsize);
fListNode* newfListNode(uint32_t data, uint32_t count, uint8_t internal);
//void fListInsert(fList* list, uint32_t data, uint32_t count);
void fListInsert(fList* list, fListNode* newnode);
fListNode* fListRemove(fList* list, fListNode* node);
fListNode* fListMin(fList* list);
fListNode* fListMax(fList* list);

void printall();


void dfsTree(uint32_t* codes, uint32_t* bitcount, fListNode* node, int bits, uint32_t repr);
#endif