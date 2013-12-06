#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "heap.h"

#ifndef STACK_H
#define STACK_H

typedef struct stackelem {
	heapNode* this;
	struct stackelem* next;
	uint32_t code;
	uint8_t codelen;
} stack;

stack* newStack();
struct stackelem* newStackElem(heapNode* ptr);
void stackPush(stack* s, struct stackelem* e);
struct stackelem* stackPop(stack* s);

#endif
