SRC_FILES=main.c huff.c heap.c stack.c

all: main

main: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o ../huffman $(SRC_FILES)

clean:
	rm -rf ../huffman
	
run: 
	../huffman
