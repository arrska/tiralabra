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
	return h;
}

void huff(heap* h) {
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
}

//writes huffman tree into file
void writeHeader(FILE* compf, heapNode* root, uint8_t blocksize, uint32_t* codes, uint8_t* codelens, long filesize) {
	//blocksize
	fwrite(&blocksize, sizeof(uint8_t), 1, compf);
	
	//reserve space for filesize and leaf count
	uint32_t leaves = 0;
	fwrite(&filesize, sizeof(long), 1, compf);
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
	
	//write filesize and leaf count
	fseek(compf, loffs, SEEK_SET);
	fwrite(&leaves, sizeof(uint32_t), 1, compf);
	fseek(compf, 0, SEEK_END);
}

void writeData(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens, uint8_t blocksize) {
	uint32_t b = 0;
	uint32_t blk = 0;
	uint8_t tmpbyte=0;
	int ret = blocksize;
	int bits = 0;
	
	rewind(origf);
	
	while (ret == blocksize) {
		ret = fread(&blk, blocksize, 1, origf);
		
		bits += codelens[blk];
		b=b|(codes[blk]<<(32-bits));
		
		while (bits >= 8) {
			tmpbyte = b>>24;
			fwrite(&tmpbyte, 1, 1, compf);
			b<<=8;
			bits-=8;
		}
		//printf("buffer: 0x%02x, bits: %d\n", b, bits);
		//printf("bits: %d\n", bits);
	}
	
	while (bits > 0) {
		if (bits < 8) {
			tmpbyte = b>>(32-bits);
			fwrite(&tmpbyte, 1, 1, compf);
			break;
		}
		tmpbyte = b>>24;
		fwrite(&tmpbyte, 1, 1, compf);
		b<<=8;
		bits-=8;
	}
}

void decompress(FILE* compf, FILE* outfile) {
	uint8_t blocksize;
	fread(&blocksize, sizeof(uint8_t), 1, compf);
	
	long filesize;
	fread(&filesize, sizeof(long), 1, compf);
	
	uint32_t leaves;
	fread(&leaves, sizeof(uint32_t), 1, compf);
	
	int elems = 1<<blocksize*8;
	uint32_t* codes = calloc(elems, sizeof(uint32_t));
	uint8_t* codelens = calloc(elems, sizeof(uint8_t));
	uint32_t blk = 0;
	uint32_t tmp=0;
	
	//root of the tree
	heapNode* root = malloc(sizeof(heapNode));
	heapNode* tmpNode;
	
	while (leaves>0) {
		fread(&blk, blocksize, 1, compf);
		fread(&codelens[blk], sizeof(uint8_t), 1, compf);
		fread(&codes[blk], sizeof(uint32_t), 1, compf);
	
		heapNode* n = newHeapNode(blk, codes[blk]);
		tmp = codes[blk];
		//pos=0;
		tmpNode = root;
		for (int i=1;i<codelens[blk];i++) {
			if (tmp>>(codelens[blk]-i) & 1) {
				//drop to right and create parent, if doesnt exist
				if (tmpNode->right == NULL) {
					tmpNode->right = malloc(sizeof(heapNode));
				}
				tmpNode = tmpNode->right;
			} else {
				//drop left
				if (tmpNode->left == NULL) {
					tmpNode->left = malloc(sizeof(heapNode));
				}
				tmpNode = tmpNode->left;
			}
		}
		//last rightmost bit decides the side
		if (tmp & 1) {
			tmpNode->right=n;
		} else {
			tmpNode->left=n;
		}
		
		//fprintf(stderr, "block: %1$x (%1$c), code: 0x%2$02x, len %3$d, pos: %4$d\n", blk, codes[blk], codelens[blk], pos);
		leaves--;
	}
	
	
	uint32_t bb = 0;
	uint8_t byteb = 0;
	int bits=0;
	int ret;
	
	tmpNode = root;
	ret = fread(&byteb, 1, sizeof(uint8_t), compf);
	while (ret && filesize>0) {
		bb=byteb<<24 | bb>>8;
		
		bits+=8;
		
		while (bits>0 && filesize>0) {
			//deal one bit (leftmost)
			if (0x80000000 & bb) {
				//go right
				tmpNode = tmpNode->right;
			} else {
				//go left
				tmpNode = tmpNode->left;
			}
			bb<<=1;
			bits--;
			
			//if is leaf
			if (tmpNode->right == NULL && tmpNode->left == NULL) {
				//printf("%c", h->nodes[pos]->data);
				fwrite(&tmpNode->data, blocksize, 1, outfile);
				//fprintf(stdout, "%c", h->nodes[pos]->data);
				//fprintf(stderr, "%ld\n", filesize);
				tmpNode = root;
				filesize--;
			}
		}
		ret = fread(&byteb, 1, sizeof(uint8_t), compf);
	}
	
	free(codelens);
}


void compress(FILE* origf, FILE* compf, uint8_t blocksize) {
		uint32_t elems = 1 << blocksize*8;
		
		heap* h = loadHeap(origf, sizeof(uint8_t)*blocksize);
		huff(h);
		
		uint32_t* codes = calloc(elems, sizeof(uint32_t));
		uint8_t* codelens = calloc(elems, sizeof(uint8_t));
		
		long filesize = ftell(origf);
		
		writeHeader(compf, h->nodes[0], blocksize, codes, codelens, filesize);
		writeData(origf, compf, codes, codelens, blocksize);
}
