#ifndef HEAPTEST_H
#define HEAPTEST_H

#include "../src/heap.h"
#include "minunit.h"

char* test_heap_sort();
char* test_delete_performance();
char* test_insert_performance();
char* test_swap();
char* test_invalid_arguments();
char* test_null_and_zero_arguments();
char* test_heapify();
char* test_heapDeleteMin();
char* test_heapInsert_and_heapMin_with_one_node();
char* test_new_heap_node();
char* test_new_empty_heap();

#endif
