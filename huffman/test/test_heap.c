#include "minunit.h"
#include "../src/heap.h"

int tests_run = 0;
int tests_passed = 0;

static char * test_new_heap() {
	heap* h;
	h=newHeap(1);
	mu_assert("newHeap should not return a NULL pointer", h!=NULL);
	return 0;
}
static char * test_ifail() {
	mu_assert("im supposed to fail", 0);
	return 0;
}

static char * all_tests() {
	mu_run_test(test_new_heap);
	mu_run_test(test_ifail);
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
