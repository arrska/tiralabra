SRC_FILES=main.c huff.c heap.c stack.c

all: huffman

huffman: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o ../huffman $(SRC_FILES)

clean:
	rm -f ../huffman
	
run: 
	../huffman
