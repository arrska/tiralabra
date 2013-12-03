#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "../src/heap.h"

int tests_passed = 0;
int asserts_ok = 0;

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
	
	heapNode* n2 = newHeapNode(0x01, 123);
	int ret = heapInsert(h, n2);
	mu_assert("inserting element to full heap should return 1", ret==1);
	
	free(h->nodes);
	free(h);
	free(n);
	free(n2);
	
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
		free(n);
		n2 = heapMin(h);
		n = heapDeleteMin(h);
		mu_assert("heapMin and heapDeleteMin should always return the same node when called consecutively", n==n2);
		heapsize--;
	}
	mu_assert("heap should only give as much nodes as it contains", heapsize==0);
	mu_assert("heap's count should be 0 when everything is removed", h->count==0);
	
	free(h->nodes);
	free(h);
	return 0;
}

static char * test_heapify() {
	heap* h = newHeap(5);
	heapNode* n1 = newHeapNode(1, 1);
	heapNode* n2 = newHeapNode(2, 2);
	heapNode* n3 = newHeapNode(3, 3);
	heapNode* n4 = newHeapNode(4, 4);
	heapNode* n5 = newHeapNode(5, 5);
	
	h->nodes[0]=n4;
	n4->index=0;
	
	h->nodes[1]=n1;
	n1->index=1;
	
	h->nodes[2]=n3;
	n3->index=2;
	
	h->nodes[3]=n5;
	n5->index=3;
	
	h->nodes[4]=n2;
	n2->index=4;
	
	h->count = 5;
	
	mu_assert("heapify should succeed and return 0", heapify(h, 0)==0);
	
	/*         4 
	 *       /   \
	 *      1     3
	 *     / \
	 *    5   2   
	 * 
	 */
	 char* msg ="\
heapifying didnt work for this: \n\
	  4 \n\
	/   \\\n\
   1     3\n\
  / \\\n\
 5   2\n\
it should be like this: \n\
	  1 \n\
	/   \\\n\
   2     3\n\
  / \\\n\
 5   4\n";
	 mu_assert(msg, 
	h->nodes[0]==n1 &&
	h->nodes[1]==n2 &&
	h->nodes[2]==n3 &&
	h->nodes[3]==n5 &&
	h->nodes[4]==n4);
	/*         1 
	 *       /   \
	 *      2     3
	 *     / \
	 *    5   4   
	 * 
	 */
	 return 0;
}

static char * test_null_and_zero_arguments() {
	heap* h = newHeap(0);
	mu_assert("newHeap-call with 0 size should return a valid pointer to heap with nodes pointer being NULL", h != NULL);
	mu_assert("newHeap(0)-call should return pointer to heap with nodes pointer being NULL", h->nodes==NULL);
	
	mu_assert("heapInsert with NULL heap- and node-pointer should return 1 indicating error", heapInsert(NULL, NULL)==1);
	
	heapNode* n = newHeapNode(0xAB, 10);
	mu_assert("heapInsert with NULL heap pointer should return 1 indicating error", heapInsert(h, NULL)==1);
	mu_assert("heapInsert with NULL node pointer should return 1 indicating error", heapInsert(NULL ,n)==1);
	mu_assert("failed inserts should't increase heap's element counter", h->count==0);
	
	mu_assert("heapMin with NULL heap should return NULL", heapMin(NULL) == NULL);
	mu_assert("heapMin with empty heap should return NULL", heapMin(h) == NULL);
	
	mu_assert("heapDeleteMin with NULL heap should return NULL", heapDeleteMin(NULL) == NULL);
	mu_assert("heapDeleteMin with empty heap should return NULL", heapDeleteMin(h) == NULL);
	
	mu_assert("heapify with NULL heap should return NULL", heapify(NULL, 0) == 1);
	mu_assert("heapify with empty heap should return NULL", heapify(h, 0) == 1);
	
	mu_assert("swap should fail with null node2", swap(h, n, NULL) == 1);
	mu_assert("swap should fail with null node1", swap(h, NULL, n) == 1);
	mu_assert("swap should fail with null heap", swap(NULL, n, n) == 1);
	
	mu_assert("parentNode should fail with null heap", parentNode(NULL, 0) == NULL);
	mu_assert("leftChild should fail with null heap", parentNode(NULL, 0) == NULL);
	mu_assert("rightChild should fail with null heap", parentNode(NULL, 0) == NULL);
	
	free(h->nodes);
	free(h);
	free(n);
	
	return 0;
}

static char* test_invalid_arguments() {
	heap* h = newHeap(2);
	heapNode* n1 = newHeapNode(1, 1);
	heapNode* n2 = newHeapNode(2, 2);
	
	h->nodes[0]=n2;
	n2->index=0;
	
	h->nodes[1]=n1;
	n1->index=1;
	
	h->count=2;
	
	mu_assert("heapify should return 1 (error) when givign invalid node index as argument", heapify(h, 2)==1);
	
	mu_assert("parentNode should return NULL when index not in heap range", parentNode(h, 2)==NULL);
	mu_assert("leftChild should return NULL when index not in heap range", leftChild(h, 2)==NULL);
	mu_assert("rightChild should return NULL when index not in heap range", rightChild(h, 2)==NULL);
	
	free(h->nodes);
	free(h);
	free(n1);
	free(n2);
	
	return 0;
}

static char* test_swap() {
	heap* h = newHeap(2);
	heapNode* n1 = newHeapNode(1, 1);
	heapNode* n2 = newHeapNode(2, 2);
	
	h->nodes[0]=n2;
	n2->index=0;
	
	h->nodes[1]=n1;
	n1->index=1;
	
	h->count=2;
	
	swap(h, n1, n2);
	swap(h, n1, n2);
	swap(h, n1, n2);
	
	mu_assert("heapify should return 1 (error) when givign invalid node index as argument", 
		h->nodes[0]==n1 && 
		h->nodes[1]==n2 && 
		n1->index==0 && 
		n2->index==1);
	
	free(h->nodes);
	free(h);
	free(n1);
	free(n2);
	return 0;
}


//heap big test, random

static char * test_insert_efficiency() {
	int items = 100000;
	heap* h;
	heapNode* n;
	
    struct timeval starttime, stoptime;
	
	unsigned long test_time[4];
	
	for (int j = 0; j < 4; j++) {
		h = newHeap(items);
	
		for (int i = 0; i < items-1000; i++) {
			n=newHeapNode(1, rand());
			heapInsert(h, n);
		}
		
		
		gettimeofday(&starttime, NULL);
		for (int k = 0; k < 1000; k++) {
			n=newHeapNode(1, rand());
			heapInsert(h, n);
		}
		gettimeofday(&stoptime, NULL);
		
		
		for (int i = 0; i < items; i++) {
			if (i<5)printf("value: %d\n", h->nodes[i]->value);
			free(h->nodes[i]);
		}
		free(h->nodes);
		free(h);
		
		test_time[j] = stoptime.tv_usec-starttime.tv_usec;
		
		printf("nodes in tree: %d \n", items);
		printf("time: %lu\n", test_time[j]);
		//printf("time/insert: %d\n\n", tests[j]*1000/items);
		items*=10;
	}
	
	return 0;
}

//heap test time n log n

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
	mu_run_test(test_null_and_zero_arguments);
	mu_run_test(test_heapify);
	mu_run_test(test_swap);
	mu_run_test(test_invalid_arguments);
	
	mu_run_test(test_insert_efficiency);
	return 0;
}
 
int main(int argc, char **argv) {
	char *result = all_tests();
	printf("== Heap tests ==\n");
	
	printf("Tests passed: %d\n\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);
	
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}

	return result != 0;
}
