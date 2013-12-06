#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "test_heap.h"
#include "test_stack.h"
#include "test_huff.h"

int tests_passed = 0;
int asserts_ok = 0;


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
 
static char * stack_tests() {
	mu_run_test(test_stack_pop);
	mu_run_test(test_one_element_stack);
	mu_run_test(test_new_stack_element);
	mu_run_test(test_stack_order);
	
	return 0;
}
 
static char * huff_tests() {
	mu_run_test(test_read_bytes_everybyte);
	mu_run_test(test_read_bytes_huffman_txt);
	mu_run_test(test_load_heap_8k_file);
	mu_run_test(test_load_heap_huffman_txt);
	mu_run_test(test_build_huffman_tree);
	mu_run_test(test_huffman_codes_rand8k);
	mu_run_test(test_huffman_codes_random);
	mu_run_test(test_write_header);
	mu_run_test(test_write_data);
	mu_run_test(test_read_header);
	mu_run_test(test_read_data);
	mu_run_test(test_rebuild_tree);
	mu_run_test(test_compress_performance);
	mu_run_test(test_decompress_performance);
	
	return 0;
}

int main(int argc, char **argv) {
	char *stack_result;
	
	
	char *huff_result;
	tests_passed = 0;
	asserts_ok = 0;
	
	
	printf("== Huffman tests ==\n");
	huff_result = huff_tests();
	
	printf("Tests passed: %d\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);
	
	if (huff_result != 0) {
		printf("%s\n\n", huff_result);
		printf("!!! ERROR ON HUFFMAN TESTS !!!\n\n");
	} else {
		printf("ALL HUFFMAN TESTS PASSED\n\n");
	}
	
	printf("== Stack tests ==\n");
	stack_result = stack_tests();
	
	printf("Tests passed: %d\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);
	
	if (stack_result != 0) {
		printf("%s\n\n", stack_result);
		printf("!!! ERROR ON STACK TESTS !!!\n\n");
	} else {
		printf("ALL STACK TESTS PASSED\n\n");
	}
	
	return 0;
	
	char *heap_result;
	tests_passed = 0;
	asserts_ok = 0;
	
	
	printf("== Heap tests ==\n");
	heap_result = heap_tests();
	
	printf("Tests passed: %d\n", tests_passed);
	printf("Assertions passed: %d\n\n", asserts_ok);
	
	if (heap_result != 0) {
		printf("%s\n\n", heap_result);
		printf("!!! ERROR ON HEAP TESTS !!!\n\n");
	} else {
		printf("HEAP TESTS PASSED\n\n");
	}
	

	return stack_result != 0 || heap_result != 0 || huff_result != 0;
}
