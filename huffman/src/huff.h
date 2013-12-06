#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "heap.h"

#ifndef HUFF_H
#define HUFF_H

void compress(FILE* origf, FILE* compf) ;
uint32_t* read_bytes(FILE* file) ;
heap* load_heap(uint32_t* bytes) ;
heapNode* build_huffman_tree(heap* h) ;
void huffman_codes(heapNode* root, uint32_t* codes, uint8_t* codelens) ;
void write_header(FILE* compf, uint32_t* codes, uint8_t* codelens, long filesize) ;
void write_data(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens) ;

void decompress(FILE* compf, FILE* outfile) ;
void read_header(FILE* compf, long* filesize, uint32_t* codes, uint8_t* codelens) ;
heapNode* rebuild_tree(uint32_t* codes, uint8_t* codelens) ;
void read_data(FILE* compf, FILE* outfile, heapNode* root, long filesize) ;

#endif
