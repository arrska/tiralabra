#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "heap.h"
#include "huff.h"

void print_help(char* binname) {
	printf("\
usage: %s d | c FILE [FILE2]\n\
compresses or decompresses FILE\n\
    d decompress FILE and output to FILE2\n\
    c compress FILE to FILE2\n\
    FILE2 defaults to stdout\n", binname);
}

int main(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		print_help(argv[0]);
		return 1;
	}
	
	if (*argv[1] == 'd') {
		FILE* cf;
		FILE* outf;
		
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		
		if (access(argv[2], R_OK) != 0) {
			printf("Could not read file!\n");
			return 1;
		}
		
		cf = fopen(argv[2], "r");
		if (cf == NULL || outf == NULL) {
			printf("Error opening files!\n");
			return 1;
		}
		
		decompress(cf, outf);
	} else if(*argv[1] == 'c') {
		FILE* origf;
		FILE* outf;
		
		if(argc == 4) {
			outf = fopen(argv[3], "w");
		} else {
			outf = stdout;
		}
		
		if (access(argv[2], R_OK) != 0) {
			printf("Could not read file!\n");
			return 1;
		}
		
		origf = fopen(argv[2], "r");
		
		if (origf == NULL || outf == NULL) {
			printf("Error opening files!\n");
			return 1;
		}
		
		compress(origf, outf);
	} else {
		print_help(argv[0]);
		return 1;
	}
  return 0;
}
