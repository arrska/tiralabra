#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef TREE_H
#define TREE_H

typedef struct node {
    struct node* left;
    struct node* right;
	unsigned int data;
    unsigned int value;
} tree;

tree* buildTree(FILE* file, int blockSize);

#endif

