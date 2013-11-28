#include "stack.h"

struct stackelem* stackPop(stack* s) {
	if (s == NULL || s->next == NULL) return NULL;
	struct stackelem* e = s->next;
	s->next = e->next;
	return e;
}

void stackPush(stack* s, struct stackelem* e) {
	if (s == NULL || e == NULL) return;
	e->next = s->next;
	s->next = e;
}

struct stackelem* newStackElem(void* ptr) {
	struct stackelem* e = malloc(sizeof(struct stackelem));
	e->this = ptr;
	e->code=0;
	e->codelen=0;
	return e;
}
