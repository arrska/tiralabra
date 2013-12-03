#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "huff.h"

void print_help(char* binname) {
	printf("usage: %s d | c FILE [FILE2]\n  compresses or decompresses FILE\n   d decompress FILE and output to FILE2\n   c compress FILE to FILE2\nFILE2 defaults to stdout\n", binname);
}

int main(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		print_help(argv[0]);
		return 1;
	}
	
	if (*argv[1] == 'd') {
		FILE* cf = fopen(argv[2], "r");
		FILE* outf;
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		
		if (cf == NULL || outf == NULL) {
			printf("File not found!\n");
			return 1;
		}
		
		decompress(cf, outf);
	} else if(*argv[1] == 'c') {
		FILE* origf = fopen(argv[2], "r");
		int blocksize = 1;
		
		FILE* outf;
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		
		if (origf == NULL || outf == NULL) {
			printf("File not found!\n");
			return 1;
		}
		
		compress(origf, outf, blocksize);
	} else {
		print_help(argv[0]);
		return 1;
	}
  return 0;
}
