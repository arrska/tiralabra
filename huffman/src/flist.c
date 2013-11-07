#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include "flist.h"

fList* newFreqList(uint8_t unitsize) {
	fList* list = malloc(sizeof(fList));
	list->unitsize = unitsize;
	list->count = 1 << unitsize*8;// pow(2, unitsize);
	list->first = NULL;
	
	return list;
}

void fListInsert(fList* list, uint32_t data, uint32_t count) {
	fListNode* newnode = malloc(sizeof(fListNode));
	
	newnode->data = data;
	newnode->count = count;
	
	if (list->first == NULL) {
		newnode->next = newnode;
		newnode->prev = newnode;
		list->first = newnode;
		
		return;
	}
	
	fListNode* node = list->first;
	
	while (node->count < count) {
		node = node->next;
		
		if (node == list->first) break;
	}
	
	if (node == list->first && node->count >= count)
		list->first = newnode;
	
	newnode->prev = node->prev;
	newnode->next = node;
	
	node->prev->next = newnode;
	node->prev = newnode;
}

void printall(fList* list) {
	printf("List unit size: %d\n", list->unitsize);
	printf("List item count: %ld\n\n", list->count);
	
	fListNode* n = list->first;
	
	do {
		printf("0x%02x %d\n", n->data, n->count);
		
		n = n->next;
	} while (n != list->first);
}

void fListRemove(fList* list, fListNode* node) {
	list->count--;
	
	if (node->next == node) {
		//must be only node
		list->first = NULL;
	} else { //not only node
		if (node == list->first) {
			//first
			list->first = node->next;
		}
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	
	free(node);
}

fListNode* fListMin(fList* list) {
	if (list == NULL) return NULL;
	return list->first;
}

//useless...?
fListNode* fListMax(fList* list) {
	if (list == NULL) return NULL;
	if (list->first == NULL) return NULL;
	return list->first->prev;
}