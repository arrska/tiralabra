#ifndef HUFFTEST_H
#define HUFFTEST_H
#define _GNU_SOURCE

#include "../src/huff.h"
#include "minunit.h"

char* test_read_bytes_everybyte();
char* test_read_bytes_huffman_txt();
char* test_load_heap_8k_file();
char* test_load_heap_huffman_txt();
char* test_build_huffman_tree();
char* test_huffman_codes_rand8k();
char* test_huffman_codes_random();
char* test_write_header();
char* test_write_data();
char* test_read_header();
char* test_read_data();
char* test_rebuild_tree();
char* test_compress_performance();
char* test_decompress_performance();

#endif
