#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "test_huff.h"

int tests_passed;
int asserts_ok;



//helper
void filewriter() {
	FILE* fw = fopen("testfiles/rand_8k", "w");
	uint8_t byte;
	for (int i = 0;i<8192;i++) {
		byte = rand();
		fwrite(&byte, 1, 1, fw);
	}
	fclose(fw);
}

char* test_read_bytes_huffman_txt() {
	FILE* f = fopen("testfiles/huffman.txt", "r");
	
	uint32_t* bytes = read_bytes(f);
	
	for (int i = 0; i < 256; i++) {
		if (bytes[i] > 0) {
			if (i != 'h' && i != 'u' && i != 'f' && i != 'm' && i != 'a' && i != 'n') {
				mu_assert("read_bytes on huffman.txt should only find characters h, u, f, m, a and n", 0);
			}
		}
	}
	
	mu_assert("read _bytes should find 16 h-characters from huffman.txt", bytes['h']==16);
	mu_assert("read _bytes should find 1 u-characters from huffman.txt", bytes['u']==1);
	mu_assert("read _bytes should find 2 f-characters from huffman.txt", bytes['f']==2);
	mu_assert("read _bytes should find 1 m-characters from huffman.txt", bytes['m']==1);
	mu_assert("read _bytes should find 1 a-characters from huffman.txt", bytes['a']==1);
	mu_assert("read _bytes should find 1 n-characters from huffman.txt", bytes['n']==1);
	

	fclose(f);
	free(bytes);
	return 0;
}

char* test_read_bytes_everybyte() {
	FILE* f = fopen("testfiles/everybyte", "r");
	
	uint32_t* bytes = read_bytes(f);
	
	for (int i = 0; i < 256; i++) {
		mu_assert("read_bytes should have found every byte once", bytes[i]==1);
	}

	fclose(f);
	free(bytes);
	return 0;
}

char* test_load_heap_8k_file() {
	FILE* f = fopen("testfiles/rand_8k", "r");
	
	uint32_t* bytes = read_bytes(f);
	heap* h = load_heap(bytes);
	
	mu_assert("load_heap shouldnt return NULL pointer", h != NULL);
	mu_assert("load_heap should return heap with size of 256", h->size==256);
	mu_assert("load_heap should return heap with count of 256 for rand_8k-file", h->count==256);
	
	heapNode* n;
	int sum = 0;
	for(int i=0;i<256;i++) {
		n = heapDeleteMin(h);
		sum += n->value;
		mu_assert("delete min on rand_8k-heap should not return NULL-pointers for first 256 deletes", n != NULL);
		mu_assert("heap loaded with load_heap should not include nodes with value less than 1", n->value >= 1);
		free(n);
	}
	n = heapDeleteMin(h);
	mu_assert("after 256 deletes, delete min on rand_8k-heap should return NULL and heap should be empty", n == NULL && h->count==0);
	mu_assert("sum of deleted items from heap should be 8192 on rand_8k-file", sum==8192);
	
	free(h->nodes);
	free(h);
	fclose(f);
	free(bytes);
	return 0;
}

char* test_load_heap_huffman_txt() {
	FILE* f = fopen("testfiles/huffman.txt", "r");
	
	uint32_t* bytes = read_bytes(f);
	heap* h = load_heap(bytes);
	
	mu_assert("load_heap should return heap with count of 6 for huffman_txt-file", h->count==6);
	
	heapNode* n;
	int sum = 0;
	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 1st element from heap should be {data: 'a', value: 1}", n->data=='a' && n->value==1);
	sum += n->value;
	free(n);

	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 2nd element from heap should be {data: 'u', value: 1}", n->data=='u' && n->value==1);
	sum += n->value;
	free(n);

	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 3rd element from heap should be {data: 'n', value: 1}", n->data=='n' && n->value==1);
	sum += n->value;
	free(n);

	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 4th element from heap should be {data: 'm', value: 1}", n->data=='m' && n->value==1);
	sum += n->value;
	free(n);

	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 5th element from heap should be {data: 'f', value: 2}", n->data=='f' && n->value==2);
	sum += n->value;
	free(n);

	n = heapDeleteMin(h);
	mu_assert("load_heap on huffman.txt: 6th element from heap should be {data: 'h', value: 16}", n->data=='h' && n->value==16);
	sum += n->value;
	free(n);

	
	mu_assert("sum of deleted items from heap should be 22 on huffman_txt-filee", sum==22);
	
	free(h->nodes);
	free(h);
	fclose(f);
	free(bytes);
	return 0;
}

char* test_build_huffman_tree() {
	FILE* f = fopen("testfiles/rand_8k", "r");
	
	uint32_t* bytes = read_bytes(f);
	heap* h = load_heap(bytes);
	
	heapNode* root = build_huffman_tree(h);
	
	mu_assert("build_huffman_tree shouldn't return NULL pointer with valid arguments", root != NULL);
	mu_assert("build_huffman_tree should return tree with root that has value of 8k with rand_8k-file", root->value==8192);
	mu_assert("build_huffman_tree should return tree with root that has no data", root->data==-1);
	mu_assert("build_huffman_tree should return tree with root that has left child", root->left != NULL);
	mu_assert("build_huffman_tree should return tree with root that has right child", root->right != NULL);
	mu_assert("build_huffman_tree should have emptied the heap it was given", h->count==0);
	
	mu_assert("build_huffman_tree hould return tree with leftmost leaf of value 42 and data 0xab",
	 root->left->left->left->left->left->left->left->data==0xab &&
	 root->left->left->left->left->left->left->left->value==43);
	
	free(h->nodes);
	free(h);
	fclose(f);
	free(bytes);
	return 0;
}

char* test_huffman_codes_rand8k() {
	FILE* f = fopen("testfiles/rand_8k", "r");
	
	uint32_t* bytes = read_bytes(f);
	heap* h = load_heap(bytes);
	
	heapNode* root = build_huffman_tree(h);
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	
	huffman_codes(root, codes, codelens);
	
	mu_assert("huffman_codes, rand_8k: code of 0xab should be 0 and length 7", codes[0xab]==0 && codelens[0xab]==7);
	mu_assert("huffman_codes, rand_8k: code of 0x2c should be 110111110 and length 9", codes[0x2c]==0b110111110 && codelens[0x2c]==9);
	mu_assert("huffman_codes, rand_8k: code of 0x48 should be 11110101 and length 8", codes[0x48]==0b11110101 && codelens[0x48]==8);
	
	free(h->nodes);
	free(h);
	fclose(f);
	free(bytes);
	return 0;
}

char* test_huffman_codes_random() {
	int len = 1048576;
	//uint8_t* data = malloc(sizeof(uint8_t)*(len+1));
	FILE* f = fopen("testfiles/temp/data1M_tmp", "w+");
	
	uint8_t byte;
	
	for (int i = 0;i<len;i++) {
		if (i<8192) {
			byte = 0x61;
		} else {
			byte = rand();
		}
		fwrite(&byte, 1, 1, f);
	}
	
	fseek(f, 0, SEEK_SET);
	
	uint32_t* bytes = read_bytes(f);
	heap* h = load_heap(bytes);
	
	heapNode* root = build_huffman_tree(h);
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	
	huffman_codes(root, codes, codelens);
	
	for (int i = 0; i < 256; i++) {
		if (codelens[i]<codelens[0x61]) {
			mu_assert("huffman_codes, random data: 0x61 should have the shortest code", 0);
		}
	}
	
	free(h->nodes);
	free(h);
	fclose(f);
	free(bytes);
	return 0;
}

char* test_write_header() {
	uint8_t expected_header[18] = {
		  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //filesize as long
		  0x01, 0x00, 0x00, 0x00, //number of leafs as 32-bit int
		  0x12, //byte
		  0x04, //length of coding of byte
		  0x0D, 0x00, 0x00, 0x00 //coding of byte
	};
	
	FILE* f = fopen("testfiles/temp/header_tmp", "w+");
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	long filesize=1024;
	
	codes[0x12]=0b1101;
	codelens[0x12]=4;
	
	write_header(f, codes, codelens, filesize);
	
	mu_assert("file cursor should point to the end of the file after writing header", ftell(f)==18);
	
	fseek(f, 0, SEEK_SET);
	
	int ret = 1;
	int offs = 0;
	uint8_t byte = 0;
	while (1) {
		ret = fread(&byte, 1, 1, f);
		if (ret == 0) {
			break;
		}
		
		mu_assert("write_header produced wrong header", byte==expected_header[offs]);
		offs++;
	}
	
	fclose(f);
	return 0;
}


char* test_write_data() {
	uint8_t expected_data[16] = {
	  0xD6, 0x79, 0xE7, 0x9E, 0x79, 0xF7, 0x75, 0x55, 
	  0x5D, 0xDD, 0xD9, 0xE7, 0x9F, 0x5D, 0x67, 0x9C
	};

	FILE* srcf = fopen("testfiles/threebytes", "r");
	FILE* f = fopen("testfiles/temp/datafile_tmp", "w+");
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	
	codes[0x12]=0b1101;
	codelens[0x12]=4;
	
	codes[0x61]=0b01;
	codelens[0x61]=2;
	
	codes[0x44]=0b100111;
	codelens[0x44]=6;
	
	write_data(srcf, f, codes, codelens);
	
	
	fseek(f, 0, SEEK_SET);
	
	int ret = 1;
	int offs = 0;
	uint8_t byte = 0;
	while (1) {
		ret = fread(&byte, 1, 1, f);
		if (ret == 0) {
			break;
		}
		
		mu_assert("write_data produced wrong data", byte==expected_data[offs]);
		offs++;
	}
	
	fclose(f);
	fclose(srcf);
	free(codes);
	free(codelens);
	return 0;
}

char* test_read_header() {
	char* fn1 = "testfiles/rand_8k";
	char* fn2 = "testfiles/temp/rand_8k.txt.compressed_tmp";
	
	mu_assert("test_read_header: testfiles not accessible", access(fn1, R_OK)==0);
	
	
	FILE* fo = fopen(fn1, "r");
	FILE* fw = fopen(fn2, "w");
	
	
	uint32_t* bytes;
	bytes = read_bytes(fo);
	
	heap* h;
	h = load_heap(bytes);
	
	heapNode* root;
	root = build_huffman_tree(h);
	
	uint32_t* codes1 = calloc(256, sizeof(uint32_t));
	uint8_t* codelens1 = calloc(256, sizeof(uint8_t));
	
	huffman_codes(root, codes1, codelens1);
	
	rewind(fo);
	
	compress(fo, fw);
	
	long filesize = 0;
	fclose(fw);
	fclose(fo);
	
	fw = fopen(fn2, "r");
	uint32_t* codes2 = calloc(256, sizeof(uint32_t));
	uint8_t* codelens2 = calloc(256, sizeof(uint8_t));
	read_header(fw, &filesize, codes2, codelens2);
	
	mu_assert("read codes are different from the written ones", memcmp(codes1, codes2, 256*sizeof(uint32_t))==0);
	mu_assert("read codelens are different from the written ones", memcmp(codelens1, codelens2, 256*sizeof(uint8_t))==0);
	
	
	fclose(fw);
	free(codes1);
	free(codes2);
	free(codelens1);
	free(codelens2);
	
	free(root);
	free(h);
	return 0;
}

char* test_read_data() {
	char* fn1 = "testfiles/rand_8k";
	char* fn2 = "testfiles/temp/rand_8k.compressed_tmp";
	char* fn3 = "testfiles/temp/rand_8k.back_tmp";
	
	mu_assert("test_read_data: testfiles not accessible", access(fn1, R_OK)==0);
	
	
	FILE* fo = fopen(fn1, "r");
	FILE* fw = fopen(fn2, "w");
	FILE* fb = fopen(fn3, "w");
	
	compress(fo, fw);
	
	long filesize = 0;
	fclose(fw);
	fclose(fo);
	
	fw = fopen(fn2, "r");
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	read_header(fw, &filesize, codes, codelens);
	
	heapNode* root;
	root = rebuild_tree(codes, codelens);
	
	read_data(fw, fb, root, filesize);
	
	fclose(fw);
	fclose(fb);
	
	fw = fopen(fn1, "r");
	fb = fopen(fn3, "r");
	
	int ret = 1;
	uint8_t byte1;
	uint8_t byte2;
	while (1) {
		ret = fread(&byte1, 1, 1, fw);
		ret += fread(&byte2, 1, 1, fb);
		if (ret != 2) {
			break;
		}
		
		mu_assert("files didnt match", byte1==byte2);
	}
	
	fclose(fw);
	free(codes);
	free(codelens);
	
	free(root);
	return 0;
}
char* test_rebuild_tree() {
	char* fn1 = "testfiles/threebytes";
	char* fn2 = "testfiles/temp/threebytes.compressed";
	mu_assert("test_read_data: testfiles not accessible", access(fn1, R_OK)==0);
	
	FILE* fo = fopen(fn1, "r");
	FILE* fw = fopen(fn2, "w");
	compress(fo, fw);
	
	fclose(fo);
	fclose(fw);
	
	fo = fopen(fn2, "r");
	
	uint32_t* codes = calloc(256, sizeof(uint32_t));
	uint8_t* codelens = calloc(256, sizeof(uint8_t));
	long filesize;
	
	read_header(fo, &filesize, codes, codelens);
	heapNode* root;
	root = rebuild_tree(codes, codelens);
	
	mu_assert("test_rebuild_tree: tree was not rebuilt correctly", root->left->data==0x44);
	mu_assert("test_rebuild_tree: tree was not rebuilt correctly", root->right->left->data==0x12);
	mu_assert("test_rebuild_tree: tree was not rebuilt correctly", root->right->right->data==0x61);
	
	free(root);
	free(codes);
	free(codelens);
	fclose(fo);
	return 0;
}

char* test_compress_performance() {
	char* fn = "testfiles/temp/data_tmp";
	FILE* f = fopen(fn, "w+");
	FILE* dn = fopen("/dev/null", "w");
	
	uint8_t byte;
	uint32_t len = 1<<10; //1024
	
	int tests = 15;
    struct timeval starttime, stoptime;
	long test_time[tests];
	for (int j = 0; j < tests; j++) {
		//write random data file
		for (int i = 0;i<len;i++) {
			byte = rand();
			fwrite(&byte, 1, 1, f);
		}
	
		//compress it
		rewind(f);
		
		gettimeofday(&starttime, NULL);
		compress(f, dn);
		gettimeofday(&stoptime, NULL);
		
		test_time[j] = ((stoptime.tv_sec - starttime.tv_sec)*1000000L+stoptime.tv_usec) - starttime.tv_usec;
		
		//printf("stop: %ld, start: %ld\n", stoptime.tv_usec, starttime.tv_usec);
		//printf("filesize: %4dK, time: %lu \n", len/1024, test_time[j]);
		len<<=1;
	}
	
	double sum = 0;
	for(int i=0;i<tests-1;i++) {
		sum+=(double)test_time[i+1]/(double)test_time[i];
	}
	
	//printf("average increse: %f\n", sum/tests);
	mu_assert("compression should take time relative to file compressed", sum/tests<=2.1);
	
	return 0;
}

char* test_decompress_performance() {
	char* fn1 = "testfiles/temp/data_tmp";
	char* fn2 = "testfiles/temp/compresseddata_tmp";
	char* fn3 = "/dev/null";
	FILE* f; //= fopen(fn1, "w");
	FILE* cf; // = fopen(fn2, "w");
	FILE* dn;
	//FILE* dn = fopen("/dev/null", "w");
	
	uint8_t byte;
	uint32_t len = 1<<15;
	
	int tests = 10;
    struct timeval starttime, stoptime;
	long test_time[tests];
	for (int j = 0; j < tests; j++) {
		//write random data file
		
		f = fopen(fn1, "w");
		for (int i = 0;i<len;i++) {
			byte = rand();
			fwrite(&byte, 1, 1, f);
		}
		fclose(f);
	
		//compress it
		f = fopen(fn1, "r");
		cf = fopen(fn2, "w");
		compress(f, cf);
		
		fclose(cf);
		fclose(f);
		
		cf = fopen(fn2, "r");
		dn = fopen(fn3, "w");
		//decompress it
		
		gettimeofday(&starttime, NULL);
		decompress(cf, dn);
		gettimeofday(&stoptime, NULL);
		
		fclose(dn);
		fclose(cf);
		
		test_time[j] = ((stoptime.tv_sec - starttime.tv_sec)*1000000L+stoptime.tv_usec) - starttime.tv_usec;

		//printf("stop: %ld, start: %ld\n", stoptime.tv_usec, starttime.tv_usec);
		//printf("filesize: %4dK, time: %lu \n", len/1024, test_time[j]);
		len<<=1;
	}
	
	double sum = 0;
	for(int i=0;i<tests-1;i++) {
		sum+=(double)test_time[i+1]/(double)test_time[i];
	}
	
	//printf("average increse: %f\n", sum/tests);
	mu_assert("decompression should take time relative to file compressed", sum/tests<=2.1);
	
	return 0;
}
