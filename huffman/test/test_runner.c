#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "test_stack.h"
#include "test_heap.h"

int tests_passed = 0;
int asserts_ok = 0;

static char * stack_tests() {
	mu_run_test(test_stack_pop);
	mu_run_test(test_stack_push);
	mu_run_test(test_new_stack_element);
	
	return 0;
}

static char * heap_tests() {
	mu_run_test(test_new_empty_heap);
	mu_run_test(test_new_heap_node);
	mu_run_test(test_heapDeleteMin);
	mu_run_test(test_heapInsert_and_heapMin_with_one_node);
	mu_run_test(test_null_and_zero_arguments);
	mu_run_test(test_heapify);
	mu_run_test(test_swap);
	mu_run_test(test_invalid_arguments);
	
	mu_run_test(test_insert_performance);
	mu_run_test(test_delete_performance);
	mu_run_test(test_heap_sort);
	
	return 0;
}
 
int main(int argc, char **argv) {
	char *stack_result;
	
	printf("== Stack tests ==\n");
	stack_result = stack_tests();
	
	if (stack_result != 0) {
		printf("%s\n\n", stack_result);
	} else {
		printf("STACK TESTS PASSED\n\n");
	}
	
	printf("Tests passed: %d\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);
	
	return 0;
	
	char *heap_result;
	tests_passed = 0;
	asserts_ok = 0;
	
	
	printf("== Heap tests ==\n");
	heap_result = heap_tests();
	
	if (heap_result != 0) {
		printf("%s\n\n", heap_result);
	} else {
		printf("HEAP TESTS PASSED\n\n");
	}
	
	printf("Tests passed: %d\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);

	return stack_result != 0 || heap_result != 0;
}
