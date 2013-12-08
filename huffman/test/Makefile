SRC_FILES=test_runner.c test_heap.c ../src/heap.c test_stack.c ../src/stack.c test_huff.c ../src/huff.c

all: test

test: testhuff

testhuff: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o ../testhuff $(SRC_FILES)

clean:
	rm -f ../testhuff
	rm -rf testfiles/temp
	mkdir testfiles/temp

run:
	../testhuff

