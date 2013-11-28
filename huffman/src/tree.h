#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef TREE_H
#define TREE_H

typedef struct treenode {
	struct treenode* left;
	struct treenode* right;
} treeNode;


tree* buildTree(uint32_t* codes, uint8_t codelens);

#endif

