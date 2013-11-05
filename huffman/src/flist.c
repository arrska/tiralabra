#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include "flist.h"

fList* newFreqList(uint8_t unitsize) {
	fList* list = malloc(sizeof(fList));
	list->unitsize = unitsize;
	list->count = 256;// pow(2, unitsize);
	list->first = NULL;
	
	return list;
}

void fListInsert(fList* list, uint32_t value, uint32_t count) {
	fListNode* newnode = malloc(sizeof(fListNode));
	
	if (list->first == NULL) {
		list->first = newnode;
		newnode->value = value;
		newnode->count = count;
		
		return;
	}
	
	newnode->value = value;
	newnode->count = count;
	
	fListNode* node = list->first;
	while (node->next != NULL && node->count < newnode->count) {
		node = node->next;
	}
	
	newnode->prev = node->prev;
	newnode->next = node;
	if (node->prev != NULL) {
		node->prev->next = newnode;
	} else {
		list->first = newnode;
	}
	node->prev = newnode;
}

void printall(fList* list) {
	printf("List unit size: %d\n", list->unitsize);
	printf("List item count: %d\n\n", list->count);
	
	fListNode* n = list->first;
	
	while (n != NULL) {
		printf("0x%x %d\n", n->value, n->count);
		
		n = n->next;
	}
}

//void fListRemove();

//fListNode* fListMin();
//fListNode* fListMax();