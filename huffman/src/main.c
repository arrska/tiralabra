#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "huff.h"

int main(int argc, char *argv[]) {
	if (argc != 3) {
	printf("%d\n", argc);
		printf("usage: %s x | c FILE\n  compresses or extracts FILE\n   x extract\n   c compress\n", argv[0]);
		return 1;
	}

	char* c = argv[1];
	
	if (*c == 'x') {
		FILE* cf = fopen(argv[2], "r");
		readData(cf);
		return 0;
	} else if(*c == 'c') {
		FILE* f = fopen(argv[2], "r");
		int block = 1;
		uint32_t elems = 1 << block*8;
		
		char* filen = calloc(strlen(argv[2])+4, sizeof(char));
		strcpy(filen, argv[2]);
		filen = strcat(filen, ".hff");
		FILE* fw = fopen(filen, "w");
		
		heap* h = loadHeap(f, sizeof(uint8_t)*block);
		huff(h);
		uint32_t* codes = calloc(elems, sizeof(uint32_t));
		uint8_t* codelens = calloc(elems, sizeof(uint8_t));
		writeHeader(fw, h->nodes[0], block, codes, codelens);
		writeData(f, fw, codes, codelens, block);
		//uint32_t* codes = huff(h, codelens);
			
		//writeData(f, fw, codes, codelens, sizeof(uint8_t)*s);
		//emptyHeap(h);
	}
  return 0;
}
