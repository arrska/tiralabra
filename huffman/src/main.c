#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


int main() {
    FILE* f = fopen("testdata", "r");
    
    buildTree(f);

    return 0;
}

