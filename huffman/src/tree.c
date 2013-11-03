#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


uint8_t least(int bytes[256]) {
	uint8_t l = 0;
	int count = bytes[0];
	
	for (int i = 1;i<256;i++)
	if (bytes[i] < count) {
		l = i;
		count = bytes[i];
	}
	
	return l;
}

uint8_t most(int bytes[256]) {
	uint8_t m = 0;
	int count = bytes[0];
	
	for (int i = 1;i<256;i++)
	if (bytes[i] > count) {
		m = i;
		count = bytes[i];
	}
	
	return m;
}

tree* buildTree(FILE* file) {
	if (file == NULL)
		return NULL;
	
	int bytes[256] ={0};

	int c = fgetc(file);
	
	while (c != EOF) {
		printf("%c", c);
		bytes[c]++;
		c = fgetc(file);
	}
	
	for (int i = 1;i<256;i++)
		if (bytes[i] != 0)
			printf("0x%x  %d\n", i, bytes[i]);
		
	uint8_t byte = most(bytes);
	printf("most of this: 0x%x, this many: %d\n", byte, bytes[byte]);
	
	byte = least(bytes);
	printf("least of this: 0x%x, this many: %d\n", byte, bytes[byte]);
	
	return NULL;
}