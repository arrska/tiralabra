#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"
#include "huff.h"

int main(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		uint8_t t = 'a';
		
		fwrite(&t, 1, 1, stdout);
		t = 0x00;
		fwrite(&t, 1, 1, stdout);
		t = 'c';
		fwrite(&t, 1, 1, stdout);
		return 1;
		printf("usage: %s x | c FILE [FILE2]\n  compresses or decompresses FILE\n   x decompress FILE and output to FILE2\n   c compress\n", argv[0]);
		return 1;
	}

	char* c = argv[1];
	
	if (*c == 'x') {
		FILE* cf = fopen(argv[2], "r");
		FILE* outf;
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		decompress(cf, outf);
	} else if(*c == 'c') {
		FILE* f = fopen(argv[2], "r");
		int blocksize = 1;
		
		char* filen = calloc(strlen(argv[2])+4, sizeof(char));
		strcpy(filen, argv[2]);
		filen = strcat(filen, ".hff");
		FILE* fw = fopen(filen, "w");
		
		compress(f, fw, blocksize);
	}
  return 0;
}
