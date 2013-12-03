SRC_FILES=test_heap.c ../src/heap.c

all: test

test: $(SRC_FILES)
	gcc -std=c99 -g -Wall -o $@ $(SRC_FILES)

clean:
	rm -rf test

run:
	./test

