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
	mu_assert("new heapNode should not have left child -pointer set. its only for tree mode", n->left == NULL);
	mu_assert("new heapNode should not have right child -pointer set. its only for tree mode", n->right == NULL);
	free(n);
	return 0;
}

static char* test_heapInsert_and_heapMin_with_one_node() {
	heap* h = newHeap(1);
	heapNode* n = newHeapNode(0x01, 123);
	heapInsert(h, n);
	
	mu_assert("first element of nodearray should be the only node inserted to heap", h->nodes[0]==n);
	mu_assert("only elemenent should be returned by heapmin with heap with only one element", heapMin(h)==n);
	mu_assert("heap count should match the number of elements in heap (1)", h->count==1);
	return 0;
}


static char* test_heapDeleteMin() {
	heap* h;
	int heapsize = 4;
	h=newHeap(heapsize);
	
	heapNode* n = newHeapNode(0x01, 123);
	heapInsert(h, n);
	n = newHeapNode(0x02, 5);
	heapInsert(h, n);
	n = newHeapNode(0x03, 20);
	heapInsert(h, n);
	n = newHeapNode(0x04, 9);
	heapInsert(h, n);
	
	mu_assert("heapMin should return the smallest inserted node", heapMin(h)->value==5);
	mu_assert("heap count should match the number of elements in heap (4)", h->count==heapsize);
	
	heapNode* n2;
	n2 = heapMin(h);
	n = heapDeleteMin(h);
	int lastValue;
	while (n!=NULL) {
		mu_assert("when emptying heap with heapDeleteMin-calls, nodes should be returned in ascending order", n->value >= lastValue);
		lastValue = n->value;
		n2 = heapMin(h);
		n = heapDeleteMin(h);
		mu_assert("heapMin and heapDeleteMin should always return the same node when called consecutively", n==n2);
		heapsize--;
	}
	mu_assert("heap should only give as much nodes as it contains", heapsize==0);
	
	return 0;
}
/*
static char* test_new_heap_nodearray_allocation() {
	uint32_t heapsize = 99;
	heap* h;
	h=newHeap(heapsize);
	mu_assert("new heap should have size of 99", h->size == heapsize);
	mu_assert("new heap should have node count of zero", h->count == 0);
	
	for (int i=0;i<heapsize;i++) {
	
	}
	
	heapNode* n1 = newHeapNode(0xAB, 1);
	heapNode* n2 = newHeapNode(0xBC, 10);
	heapNode* n3 = newHeapNode(0xCD, 100);
	
	h->nodes[0]=n1;
	h->nodes[1]=n2;
	h->nodes[98]=n3;
	
	
	
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
	mu_run_test(test_heapDeleteMin);
	mu_run_test(test_heapInsert_and_heapMin_with_one_node);
	return 0;
}
 
int main(int argc, char **argv) {
	char *result = all_tests();
	printf("== Heap tests ==\n");
	
	printf("Tests passed: %d/%d\n\n", tests_passed, tests_run);
	
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}

	return result != 0;
}
