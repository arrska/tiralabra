MAIN_DIR = src
TEST_DIR = test

all: main test run-test
main:
	$(MAKE) -C $(MAIN_DIR)

test:
	$(MAKE) -C $(TEST_DIR)

clean: clean clean-test
clean-main:
	$(MAKE) -C $(MAIN_DIR) clean
	
clean-test:
	$(MAKE) -C $(TEST_DIR) clean
	
run: run-test run-main

run-main:
	$(MAKE) -C $(MAIN_DIR) run
	
run-test:
	$(MAKE) -C $(TEST_DIR) run