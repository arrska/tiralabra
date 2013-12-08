MAIN_DIR = src
TEST_DIR = test

all: main test run-test

main: huffman

huffman:
	$(MAKE) -C $(MAIN_DIR)
	
test: testhuff
testhuff:
	$(MAKE) -C $(TEST_DIR)

clean: clean-main clean-test
clean-main:
	$(MAKE) -C $(MAIN_DIR) clean
	
clean-test:
	$(MAKE) -C $(TEST_DIR) clean
	
run: run-main

run-main:
	$(MAKE) -C $(MAIN_DIR) run
	
run-test:
	$(MAKE) -C $(TEST_DIR) run