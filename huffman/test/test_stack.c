#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_stack.h"

int tests_passed;
int asserts_ok;

char* test_stack_pop() {
	mu_assert("stack pop with NULL-pointer should return NULL", stackPop(NULL)==NULL);
	
	stack* s = malloc(sizeof(stack));
	mu_assert("empty stack pop should return NULL", stackPop(s)==NULL);
	
	free(s);
	return 0;
}

char* test_stack_push() {
	stack* s = malloc(sizeof(stack));
	struct stackelem* e = newStackElem(NULL);
	
	stackPush(s, e);
	
	mu_assert("first element in stack should be the one pushed last", s->next == e);
	mu_assert("popping only element in stack should return that element", stackPop(s) == e);
	
	return 0;
}

char* test_new_stack_element() {
	heapNode* n = newHeapNode(1,2);
	struct stackelem* e = newStackElem(n);
	
	
	mu_assert("newStackElem should set code to 0", e->code==0);
	mu_assert("newStackElem should set codelen to 0", e->code==0);
	mu_assert("newStackElem should set this-pointer to ptr", e->this==n);
	
	return 0;
}
