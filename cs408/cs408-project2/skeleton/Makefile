# ----------------------------------------------------- #
# THIS FILE SHOULD NOT BE MODIFIED IN YOUR SUBMISSION   #
# ----------------------------------------------------- #

# General
SRC = *.c
SRC_TESTS = $(wildcard tests/*.c)
TEST_PRGS = $(patsubst %.c,%,$(SRC_TESTS))
CC = gcc

# Flags
STD    = -std=gnu11
WARN   = -Wall -Wextra -Wshadow -Wno-unused-parameter

FLAGS  = $(STD) $(WARN) -O2 -g3 -fPIC -I libunwind/include/ -ldl
CFLAGS = $(FLAGS) -march=native

# Targets
library:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -c
	$(CC) -o testlib.so testlib.o utils.o libunwind/lib/libunwind*.a -shared -ldl

library_with_coverage:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -c --coverage
	$(CC) -o testlib.so testlib.o utils.o libunwind/lib/libunwind*.a -shared --coverage -ldl

$(TEST_PRGS): %: %.c
	$(CC) $(CFLAGS) -ldl -pthread -o $@ $<

tests_build: $(TEST_PRGS) library_with_coverage

test: tests_build
	python3 tests.py

clean:
	for prg in $(TEST_PRGS) ; do \
	  rm -f $$prg ; \
	done
	rm -f testlib.so
	rm -f ./*.o
	rm -f ./*.gcda
	rm -f ./*.gcov
	rm -f ./*.gcno
