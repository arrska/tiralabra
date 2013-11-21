#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "huff.h"

//without code lengths?
uint32_t* huff(heap* h, uint32_t* codelens) {
	heapNode* l;
	heapNode* r;
	heapNode* nn;
	
	while (h->count > 1) {
		l = heapDeleteMin(h);
		r = heapDeleteMin(h);
		
		nn = newHeapNode(-1, l->value + r->value);
		nn->left = l;
		nn->right = r;
		
		heapInsert(h, nn);
	}
	
	uint32_t* codes = calloc(h->size, sizeof(uint32_t));
	dfsTree(heapMin(h), codes, codelens, 0, 0);
	
	return codes;
}


//TODO: use stack and while
//without code lengths?
void dfsTree(heapNode* n, uint32_t* codes, uint32_t* codelens, uint32_t code, uint8_t codelen) {
	if (n->left == NULL && n->right == NULL) {
		codes[n->data] = code;
		codelens[n->data] = codelen;
		printf("data: 0x%04x (%c), code: 0x%04x, codelen: %d\n", n->data, n->data, code, codelen);
		return;
	}
	dfsTree(n->right, codes, codelens, (code << 1) | 1, codelen+1);
	dfsTree(n->left, codes, codelens, (code << 1), codelen+1);
}

//so sad code
void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint32_t* codelens, uint8_t block) {
	uint32_t* buff = calloc(block, sizeof(uint8_t)*block);
	//uint64_t b;
	uint64_t* b = calloc(sizeof(uint64_t), 1);
	int bits = 0;
	int ret = block;
	int elems = 1<<block*8;
	
	fwrite(&block, sizeof(uint8_t), 1, compf);
	
	for (uint32_t i = 0; i<elems; i++) {
		if (codes[i]>0) {
			fwrite(&i, sizeof(uint32_t), 1, compf);
			fwrite(&codes[i], sizeof(uint32_t), 1, compf);
		}
	}
	
	fseek(origf, 0, SEEK_SET);
	
	while (ret == block) {
		ret = fread(buff, 1, block, origf);
		*b = *b<<codelens[*buff] | codes[*buff];
		
		bits += codelens[*buff];
		//printf("read block: 0x%02x, # of bits in buffer: %d, bits: 0x%08x\n", *buff, bits, *b);
		
		if (bits >= 16) {
			uint64_t tmp = 0;
			tmp = *b;
			
			*b>>=bits-(bits/8)*8;
			fwrite(b, bits/8, 1, compf);
			//printf("wrote something: %x (first %d bytes)\n", *b, bits/8);
			
			*b=tmp>>(bits/8)*8;
			bits-=(bits/8)*8;
		}
	}
	
	fclose(origf);
	fclose(compf);
}



/*
heap* pickMin(heap* h1, heap* h2) {
	heapNode* n1 = heapMin(h1);
	heapNode* n2 = heapMin(h2);
	
	if (n2 == NULL || n1->value > n2->value) {
		return h1;
	} else {
		return h2;
	}
}
*/
