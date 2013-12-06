#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_stack.h"

int tests_passed;
int asserts_ok;

char* test_stack_pop() {
	mu_assert("stack pop with NULL-pointer should return NULL", stackPop(NULL)==NULL);
	
	stack* s = newStack();
	mu_assert("empty stack pop should return NULL", stackPop(s)==NULL);
	
	free(s);
	return 0;
}

char* test_one_element_stack() {
	stack* s = newStack();
	struct stackelem* e = newStackElem(NULL);
	
	stackPush(s, e);
	
	mu_assert("first element in stack should be the one pushed last", s->next == e);
	mu_assert("popping only element in stack should return that element", stackPop(s) == e);
	mu_assert("popping empty stack should return NULL", stackPop(s) == NULL);
	
	free(s);
	free(e);
	return 0;
}

char* test_stack_order() {
	stack* s = newStack();
	struct stackelem* e1 = newStackElem(NULL);
	struct stackelem* e2 = newStackElem(NULL);
	struct stackelem* e3 = newStackElem(NULL);
	
	stackPush(s, e1);
	stackPush(s, e2);
	stackPush(s, e3);
	
	mu_assert("popping only elements should return them in reverse order (LIFO)", stackPop(s) == e3);
	mu_assert("popping only elements should return them in reverse order (LIFO)", stackPop(s) == e2);
	mu_assert("popping only elements should return them in reverse order (LIFO)", stackPop(s) == e1);
	mu_assert("after popping all elements, stack should be null", stackPop(s) == NULL);
	
	free(s);
	free(e1);
	free(e2);
	free(e3);
	
	return 0;
}

char* test_new_stack_element() {
	heapNode* n = newHeapNode(1,2);
	struct stackelem* e = newStackElem(n);
	
	mu_assert("newStackElem should set code to 0", e->code==0);
	mu_assert("newStackElem should set codelen to 0", e->code==0);
	mu_assert("newStackElem should set this-pointer to ptr", e->this==n);
	
	free(n);
	free(e);
	return 0;
}
