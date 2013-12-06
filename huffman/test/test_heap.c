#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_heap.h"

int tests_passed;
int asserts_ok;

char* test_new_empty_heap() {
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
char* test_new_heap_node() {
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

char* test_heapInsert_and_heapMin_with_one_node() {
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


char* test_heapDeleteMin() {
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

char* test_heapify() {
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

char* test_null_and_zero_arguments() {
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

char* test_invalid_arguments() {
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

char* test_swap() {
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


char* test_insert_performance() {
	int items = 100000;
	int inserts = 1000;
	int tests=4;
	heap* h;
	heapNode* n;
	
    struct timeval starttime, stoptime;
	
	unsigned long test_time[tests];
	
	for (int j = 0; j < tests; j++) {
		h = newHeap(items+inserts);
	
		for (int i = 0; i < items; i++) {
			n=newHeapNode(1, i%200+tests);
			heapInsert(h, n);
		}
		
		
		gettimeofday(&starttime, NULL);
		for (int k = 0; k < inserts; k++) {
			n=newHeapNode(1, k%tests);
			heapInsert(h, n);
		}
		gettimeofday(&stoptime, NULL);
		
		
		for (int i = 0; i < items+inserts; i++) {
			free(h->nodes[i]);
		}
		free(h->nodes);
		free(h);
		
		test_time[j] = stoptime.tv_usec-starttime.tv_usec;
		
		//printf("nodes in tree: %d, inserts: %d \n", items, inserts);
		//printf("time: %lu\n\n", test_time[j]);
		items*=10;
	}
	
	for(int i=0;i<tests-1;i++) {
		//allow margin of 1
		mu_assert("inserting elements into small heap should be about faster than inserting them into big heap", test_time[i] <= test_time[i+1]+1);
		//printf("time1: %lu, time1: %lu, time2/time1: %lf\n", test_time[i], test_time[i+1], (double)test_time[i+1]/(double)test_time[i]);
		mu_assert("inserting elements into big heap should be not be n times slower than inserting them into n times smaller heap", (double)test_time[i+1]/(double)test_time[i]<2);
	}
	
	return 0;
}

char* test_delete_performance() {
	int items = 100000;
	int deletes = 1000;
	int tests=4;
	heap* h;
	heapNode* n;
	
    struct timeval starttime, stoptime;
	
	unsigned long test_time[tests];
	
	for (int j = 0; j < tests; j++) {
		h = newHeap(items+deletes);
	
		for (int i = 0; i < items; i++) {
			n=newHeapNode(1, i%200+tests);
			heapInsert(h, n);
		}
		
		
		gettimeofday(&starttime, NULL);
		for (int k = 0; k < deletes; k++) {
			n=heapDeleteMin(h);
		}
		gettimeofday(&stoptime, NULL);
		
		
		for (int i = 0; i < items+deletes; i++) {
			free(h->nodes[i]);
		}
		free(h->nodes);
		free(h);
		
		test_time[j] = stoptime.tv_usec-starttime.tv_usec;
		
		//printf("nodes in tree: %d, deletions: %d \n", items, deletes);
		//printf("time: %lu\n\n", test_time[j]);
		items*=10;
	}
	
	for(int i=0;i<tests-1;i++) {
		mu_assert("deleting elements from small heap should be about faster than deleteing them from big heap", test_time[i] <= test_time[i+1]);
		//printf("time1: %lu, time1: %lu, time2/time1: %lf\n", test_time[i], test_time[i+1], (double)test_time[i+1]/(double)test_time[i]);
		mu_assert("deleteing elements from big heap should be not be n times slower than deleting them from n times smaller heap", (double)test_time[i+1]/(double)test_time[i]<2);
	}
	
	return 0;
}

char* test_heap_sort() {
	int items = 10000;
	heap* h = newHeap(items);
	heapNode* n;
	
	for (int i = 0; i < items; i++) {
		n = newHeapNode(1, rand());
		heapInsert(h, n);
	}
	
	
	uint32_t last = 0;
	while (h->count>0) {
		n = heapDeleteMin(h);
		mu_assert("heap should return elements in right order", last <= n->value);
		last = n->value;
		free(n);
	}
	
	free(h->nodes);
	free(h);
	
	return 0;
}
