#include "huff.h"

//reads file block by block and counts frequencies
uint32_t* read_bytes(FILE* file) {
	if (file == NULL)
		return NULL;
	
	uint32_t* bytes = calloc(256, sizeof(uint32_t));
	uint8_t c = 0;
	
	int ret = 1;
	
	bytes[c]--;
	while (ret == 1) {
		bytes[c]++;
		c = 0;
		ret = fread(&c, 1, 1, file);
	}
	
	return bytes;
}

//build heap from byte frequencies
heap* load_heap(uint32_t* bytes) {
	heap* h = newHeap(256);
	
	for (int i = 0;i<256;i++) {
		if (bytes[i] > 0) {
			heapInsert(h, newHeapNode(i, bytes[i]));
			//printf("insert: %c, %d\n", i, bytes[i]);
		}
	}
	return h;
}

//build huffman tree from byteheap
heapNode* build_huffman_tree(heap* h) {
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
	
	return heapDeleteMin(h);
}

//traverse tree to find codes
void huffman_codes(heapNode* root, uint32_t* codes, uint8_t* codelens) {
	stack* s = malloc(sizeof(stack));
	struct stackelem* e = newStackElem(root);
	struct stackelem* r;
	struct stackelem* l;
	
	//dfs
	heapNode* n;
	
	while ( e != NULL ) {
		n = e->this;
		
		if (n->left == NULL && n->right == NULL) {
			codes[n->data] = e->code;
			codelens[n->data] = e->codelen;
			printf("data: 0x%04x (%c), code: 0x%04x, codelen: %d\n", n->data, n->data, e->code, e->codelen);
			free(e);
		} else {
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
		e = stackPop(s);
	}
}

//write codes for symbols into file header
void write_header(FILE* compf, uint32_t* codes, uint8_t* codelens, long filesize) {
	//write filesize
	uint32_t leafs = 0;
	fwrite(&filesize, sizeof(long), 1, compf);
	
	//reserve space for leaf count
	long loffs = ftell(compf);
	fwrite(&leafs, sizeof(uint32_t), 1, compf);
	
	for (int byte=0;byte<256;byte++) {
		if (codelens[byte] > 0) {
			fwrite(&byte, 1, 1, compf);
			fwrite(&codelens[byte], sizeof(uint8_t), 1, compf);
			fwrite(&codes[byte], sizeof(uint32_t), 1, compf);
			leafs++;
		}
	}
	
	//write leaf count
	fseek(compf, loffs, SEEK_SET);
	fwrite(&leafs, sizeof(uint32_t), 1, compf);
	fseek(compf, 0, SEEK_END);
}

//compress and write data to file
void write_data(FILE* origf, FILE* compf, uint32_t* codes, uint8_t* codelens) {
	uint32_t buffer = 0;
	uint8_t byte = 0;
	uint8_t tmpbyte = 0;
	int ret = 1;
	int bits = 0;
	
	rewind(origf);
	
	while (ret == 1) {
		ret = fread(&byte, 1, 1, origf);
		
		bits += codelens[byte];
		buffer = buffer|(codes[byte]<<(32-bits));
		
		while (bits >= 8) {
			tmpbyte = buffer>>24;
			fwrite(&tmpbyte, 1, 1, compf);
			buffer<<=8;
			bits-=8;
		}
	}
	
	//empty buffer
	while (bits > 0) {
		if (bits < 8) {
			tmpbyte = buffer>>(32-bits);
			fwrite(&tmpbyte, 1, 1, compf);
			break;
		}
		tmpbyte = buffer>>24;
		fwrite(&tmpbyte, 1, 1, compf);
		buffer<<=8;
		bits-=8;
	}
}

//read compressed file header
void read_header(FILE* compf, long* filesize, uint32_t* codes, uint8_t* codelens) {
	uint32_t leafs;
	
	fread(filesize, sizeof(long), 1, compf);
	fread(&leafs, sizeof(uint32_t), 1, compf);
	
	
	uint32_t byte = 0;
	
	
	while (leafs>0) {
		fread(&byte, 1, 1, compf);
		fread(&codelens[byte], sizeof(uint8_t), 1, compf);
		fread(&codes[byte], sizeof(uint32_t), 1, compf);
		
		//fprintf(stderr, "block: %1$x (%1$c), code: 0x%2$02x, len %3$d, pos: %4$d\n", blk, codes[blk], codelens[blk], pos);
		leafs--;
	}
}

//rebuild tree from codes
heapNode* rebuild_tree(uint32_t* codes, uint8_t* codelens) {
	//root of the tree
	heapNode* root = malloc(sizeof(heapNode));
	heapNode* tmpNode;
	uint32_t code=0;
	
	for(int byte = 0;byte<256;byte++) {
		if (codelens[byte] > 0) {
			heapNode* n = newHeapNode(byte, 0);
			code = codes[byte];
			tmpNode = root;
			
			//read bits from left to right
			for (int i=1;i<codelens[byte];i++) {
				if (code>>(codelens[byte]-i) & 1) {
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
			
			//rightmost(last) bit decides the side
			if (code & 1) {
				tmpNode->right=n;
			} else {
				tmpNode->left=n;
			}
		}
	}
	
	return root;
}

//read and decompress data
void read_data(FILE* compf, FILE* outfile, heapNode* root, long filesize) {
	uint32_t buffer = 0;
	uint8_t byte = 0;
	int bits=0;
	int ret;
	
	heapNode* tmpNode = root;
	ret = fread(&byte, 1, sizeof(uint8_t), compf);
	while (ret && filesize>0) {
		buffer=byte<<24 | buffer>>8;
		
		bits+=8;
		
		while (bits>0 && filesize>0) {
			//deal one bit (leftmost)
			if (0x80000000 & buffer) {
				//go right
				tmpNode = tmpNode->right;
			} else {
				//go left
				tmpNode = tmpNode->left;
			}
			buffer<<=1;
			bits--;
			
			//if is leaf
			if (tmpNode->right == NULL && tmpNode->left == NULL) {
				fwrite(&tmpNode->data, 1, 1, outfile);
				tmpNode = root;
				filesize--;
			}
		}
		ret = fread(&byte, 1, sizeof(uint8_t), compf);
	}
}

//decompress file
void decompress(FILE* compf, FILE* outfile) {
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	long filesize;
	
	read_header(compf, &filesize, codes, codelens);
	heapNode* root;
	root = rebuild_tree(codes, codelens);
	
	read_data(compf, outfile, root, filesize);
}

//compress file
void compress(FILE* origf, FILE* compf) {
	uint32_t* bytes;
	bytes = read_bytes(origf);
	
	heap* h;
	h = load_heap(bytes);
	
	heapNode* root;
	root = build_huffman_tree(h);
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	
	long filesize = ftell(origf);
	
	huffman_codes(root, codes, codelens);
	write_header(compf, codes, codelens, filesize);
	write_data(origf, compf, codes, codelens);
}
