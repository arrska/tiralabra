SRC_FILES=main.c tree.c

all: main

main: $(SRC_FILES)
	gcc -Wall -g -std=c99 -o $@ $(SRC_FILES)

clean:
	rm -rf main

run: main
	./main
