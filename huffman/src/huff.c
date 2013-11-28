#include "huff.h"

//reads file block by block, counts frequencies and builds heap
heap* loadHeap(FILE* file, uint8_t blocksize) {
	if (file == NULL)
		return NULL;
	
	if (blocksize <= 0 || blocksize > sizeof(uint32_t))
		return NULL;
	
	uint32_t elems = 1 << blocksize*8;
	//hash table maybe? at least for bigger blocks
	uint32_t* bytes = calloc(elems, sizeof(uint32_t));
	uint32_t* c = calloc(blocksize, sizeof(uint8_t));
	
	int ret = blocksize;
	
	bytes[*c]--;
	while (ret == blocksize) {
		bytes[*c]++;
		*c = 0;
		ret = fread(c, blocksize, 1, file);
	}
	//do something with last block if not full
	printf("last read: %d bytes, data: 0x%04x\n", ret, *c); 
	
	heap* h = newHeap(elems);
	
	for (int i = 0;i<elems;i++) {
		if (bytes[i] > 0) {
			heapInsert(h, newHeapNode(i, bytes[i]));
			printf("insert: %c, %d\n", i, bytes[i]); 
		}
	}
	/*
	for (int i = 0; i < h->count; i++) {
		printf("%2d val: %d data: 0x%02x (%c)\n", i, h->nodes[i]->value, h->nodes[i]->data, h->nodes[i]->data);
	}*/
	
	return h;
}


//without code lengths?
//uint32_t* 
//turns heap into huffman tree
void huff(heap* h/*, uint8_t* codelens*/) {
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
	
	//uint32_t* codes = calloc(h->size, sizeof(uint32_t));
	//dfsTree(heapMin(h), codes, codelens, 0, 0);
	
	//return codes;
}

//writes huffman tree into file
void writeHeader(FILE* compf, heapNode* root, uint8_t blocksize, uint32_t* codes, uint8_t* codelens) {
	//blocksizesize
	fwrite(&blocksize, sizeof(uint8_t), 1, compf);
	
	//reserve space for leaf count
	uint32_t leaves = 0;
	long loffs = ftell(compf);
	fwrite(&leaves, sizeof(uint32_t), 1, compf);
	
	//dfs
	stack* s = malloc(sizeof(stack));
	struct stackelem* e = newStackElem(root);
	struct stackelem* r;
	struct stackelem* l;
	
	stackPush(s, e);
	heapNode* n;
	
	while ( (e = stackPop(s)) ) {
		n = e->this;
		
		if (n->left == NULL && n->right == NULL) {
			codes[n->data] = e->code;
			codelens[n->data] = e->codelen;
			printf("data: 0x%04x (%c), code: 0x%04x, codelen: %d\n", n->data, n->data, e->code, e->codelen);
			//write data to file
			fwrite(&n->data, blocksize, 1, compf);
			//write code to file
			fwrite(&e->codelen, sizeof(uint8_t), 1, compf);
			fwrite(&e->code, sizeof(uint32_t), 1, compf);
			free(e);
			leaves++;
			continue;
		}
		
		if (n->right) {
			r = newStackElem(n->right);
			r->code = (e->code << 1) | 1;
			r->codelen = e->codelen+1;
			stackPush(s, r);
		}
		
		if (n->left) {
			l = newStackElem(n->left);
			l->code = (e->code << 1);
			l->codelen = e->codelen+1;
			stackPush(s, l);
		}
	}
	
	//write leaf count
	fseek(compf, loffs, SEEK_SET);
	fwrite(&leaves, sizeof(uint32_t), 1, compf);
	fseek(compf, 0, SEEK_END);
}

void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens, uint8_t blocksize) {
	uint32_t* readbuff = calloc(1, blocksize);
	uint64_t* writeb = calloc(sizeof(uint64_t), 1);
	int ret = blocksize;
	int bits = 0;
	int total = 0;
	
	rewind(origf);
	
	while (ret == blocksize) {
		ret = fread(readbuff, blocksize, 1, origf);
		*writeb = *writeb<<codelens[*readbuff] | codes[*readbuff];
		
		bits += codelens[*readbuff];
		total += codelens[*readbuff];
		//printf("read block: 0x%02x, # of bits in buffer: %d, bits: 0x%08x\n", *buff, bits, *b);
		
		if (bits >= 16) {
			uint64_t tmp = 0;
			tmp = *writeb;
			
			*writeb>>=bits-(bits/8)*8;
			fwrite(writeb, bits/8, 1, compf);
			//printf("wrote something: %x (first %d bytes)\n", *b, bits/8);
			
			*writeb=tmp>>(bits/8)*8;
			bits-=(bits/8)*8;
		}
	}
	
	//something smarter with the leftover bits
	if (bits) {
			fwrite(writeb, bits/8+1, 1, compf);
	}
	printf("total amount of compressed data: %d (%d bits)\n", total/8, total);
}



void readData(FILE* compf) {
	//long filesize;
	//fread(&filesize, sizeof(long), 1, compf);
	uint8_t blocksize;
	fread(&blocksize, sizeof(uint8_t), 1, compf);
	
	uint32_t leaves;
	fread(&leaves, sizeof(uint32_t), 1, compf);
	
	
	int elems = 1<<blocksize*8;
	uint32_t* codes = calloc(elems, sizeof(uint32_t));
	uint8_t* codelens = calloc(elems, sizeof(uint8_t));
	uint32_t blk = 0;
	uint32_t tmp=0;
	uint32_t pos=0;
	
	heap* h = newHeap(elems);
	
	while (leaves>0) {
		fread(&blk, blocksize, 1, compf);
		
		fread(&codelens[blk], sizeof(uint8_t), 1, compf);
		fread(&codes[blk], sizeof(uint32_t), 1, compf);
	
		heapNode* n = newHeapNode(codes[blk], codelens[blk]);
		tmp = codes[blk];
		pos=0;
		for (int i=0;i<codelens[blk];i++) {
			if (0x01 & tmp) {
				pos = (pos+1)*2-1;
				printf("1");
			} else {
				pos = (pos+1)*2;
				printf("0");
			}
			tmp=tmp>>1;
		}
		h->nodes[pos] = n;
		printf("\n");
		printf("block: %1$x (%1$c), code: 0x%2$02x, len %3$d, pos: %4$d\n", blk, codes[blk], codelens[blk], pos);
		leaves--;
	}
	
	
	uint64_t* b = calloc(1, sizeof(uint64_t));
	int bits=0;
	//to be finished
	while (fread(b, 1, sizeof(uint8_t), compf)) {
		bits+=8;
		tmp=0;
		for(int i=0;i<bits;i++) {
			tmp=*b;
			if (0x01 & tmp) {
				pos = (pos+1)*2-1;
				printf("1");
			} else {
				pos = (pos+1)*2;
				printf("0");
			}
			tmp=tmp>>1;
		}
		
	}
	//build tree
}


void compress(FILE* origf, FILE* compf, heap* h, uint8_t blocksize) {
		uint8_t* codelens = calloc(1<<blocksize*8, sizeof(uint8_t));
		uint32_t* codes = calloc(h->size, sizeof(uint32_t));//huff(h, codelens);
		
		writeHeader(compf, h->nodes[0], blocksize, codes, codelens);
		
		//writeData();
}





/*
heapNode* buildTree(uint32_t* codes, uint8_t* codelens) {
	heapNode* n = malloc(sizeof(heapNode));

	for (uint32_t i = 0; i<elems; i++) {
		if (codelens[i]>0) {
		  
		}
	}
	return n;
}*/


//so sad code
/*
void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens, uint8_t block) {
	uint32_t* buff = calloc(block, sizeof(uint8_t)*block);
	//uint64_t b;
	uint64_t* b = calloc(sizeof(uint64_t), 1);
	int bits = 0;
	int ret = block;
	int elems = 1<<block*8;
	fseek(origf, 0, SEEK_END);
	long filesize = ftell(origf);
	
	fwrite(&filesize, sizeof(long), 1, compf);
	printf("original filesize was %ld\n", filesize);
	
	long treeleaves = 0;
	for (uint32_t i = 0; i<elems; i++) {
		if (codes[i]>0) {
			treeleaves++;
		}
	}
	fwrite(&treeleaves, sizeof(long), 1, compf);
	
	fwrite(&block, sizeof(uint8_t), 1, compf);
	
	for (uint32_t i = 0; i<elems; i++) {
		if (codelens[i]>0) {
			fwrite(&i, block, 1, compf);
			fwrite(&codes[i], sizeof(uint32_t), 1, compf);
			fwrite(&codelens[i], sizeof(uint8_t), 1, compf);
		}
	}
	
	fseek(origf, 0, SEEK_SET);

	
	fclose(origf);
	fclose(compf);
}*/


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

/*
void dfsTree(heapNode* n, uint32_t* codes, uint8_t* codelens, uint32_t code, uint8_t codelen) {
	if (n->left == NULL && n->right == NULL) {
		codes[n->data] = code;
		codelens[n->data] = codelen;
		printf("data: 0x%04x (%c), code: 0x%04x, codelen: %d\n", n->data, n->data, code, codelen);
		return;
	}
	dfsTree(n->right, codes, codelens, (code << 1) | 1, codelen+1);
	dfsTree(n->left, codes, codelens, (code << 1), codelen+1);
}*/
