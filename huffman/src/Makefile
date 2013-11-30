SRC_FILES=main.c huff.c heap.c stack.c

all: main

main: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o ../$@ $(SRC_FILES)
	
run: 
	./main

clean:
	rm -rf ../main
