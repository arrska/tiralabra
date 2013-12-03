#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "huff.h"

int main(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		printf("usage: %s d | c FILE [FILE2]\n  compresses or decompresses FILE\n   d decompress FILE and output to FILE2\n   c compress FILE to FILE.hff\n", argv[0]);
		return 1;
	}

	char* c = argv[1];
	
	if (*c == 'd') {
		FILE* cf = fopen(argv[2], "r");
		FILE* outf;
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		
		if (cf == NULL || outf == NULL) {
			return 1;
		}
		
		decompress(cf, outf);
	} else if(*c == 'c') {
		FILE* f = fopen(argv[2], "r");
		int blocksize = 1;
		
		char* filen = calloc(strlen(argv[2])+4, sizeof(char));
		strcpy(filen, argv[2]);
		filen = strcat(filen, ".hff");
		FILE* fw = fopen(filen, "w");
		
		if (f == NULL || fw == NULL) {
			return 1;
		}
		
		compress(f, fw, blocksize);
	}
  return 0;
}
