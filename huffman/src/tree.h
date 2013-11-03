#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef TREE_H
#define	TREE_H

typedef struct node {
    struct node* left;
    struct node* right;
    uint8_t byte;
    int value;
} tree;

tree* buildTree(FILE* file);

#endif

