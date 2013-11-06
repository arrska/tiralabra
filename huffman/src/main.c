#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "flist.h"


int main() {
    FILE* f = fopen("testdata2", "r");
    
    buildTree(f, sizeof(uint8_t)*2);

    return 0;
}

