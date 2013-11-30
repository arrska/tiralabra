#include "minunit.h"
#include "../src/heap.h"

int tests_run = 0;
int tests_passed = 0;

static char * test_new_empty_heap() {
	uint32_t heapsize = 1;
	heap* h;
	h=newHeap(heapsize);
	mu_assert("newHeap should not return a NULL pointer", h != NULL);
	mu_assert("new heap should have size bigger than 0", h->size == heapsize);
	mu_assert("new heap should have node count of zero", h->count == 0);
	mu_assert("new heap should have initialized array for nodes", h->nodes != NULL);
	free(h->nodes);
	free(h);
	return 0;
}
static char * test_new_heap_node() {
	heapNode* n = newHeapNode(0xAB, 10);
	mu_assert("newHeapNode should not return a NULL pointer", n != NULL);
	mu_assert("new heapNode should have data ser", n->data == 0xAB);
	mu_assert("new heapNode should have value set", n->value == 10);
	mu_assert("new heapNode should have index of -1", n->index == -1);
	return 0;
}
/*
static char * test_new_heap_and_insert() {
	uint32_t heapsize = 1;
	heap* h;
	h=newHeap(heapsize);
	mu_assert("newHeap should not return a NULL pointer", h!=NULL);
	mu_assert("new heap should have size bigger than 0", h->size == heapsize);
	mu_assert("new heap should have node count of zero", h->count == 0);
	mu_assert("new heap should have initialized array for nodes", h->nodes == NULL);
	return 0;
}*/
/*
static char * test_ifail() {
	mu_assert("im supposed to fail", 0);
	return 0;
}*/

static char * all_tests() {
	mu_run_test(test_new_empty_heap);
	mu_run_test(test_new_heap_node);
	return 0;
}
 
int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests passed: %d/%d\n", tests_passed, tests_run);

	return result != 0;
}
