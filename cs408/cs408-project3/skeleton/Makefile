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
all: library tests_build
library:
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -c
	$(CC) -o testlib.so testlib.o utils.o -shared -ldl

libaccess: libaccess.c libaccess.h
	gcc -fPIC -c -o libaccess.o libaccess.c
	gcc -shared -o libaccess.so libaccess.o

$(TEST_PRGS): %: %.c libaccess
	$(CC) $(CFLAGS) -ldl -pthread -o $@ $< -L . -laccess

tests_build: $(TEST_PRGS)


clean:
	for prg in $(TEST_PRGS) ; do \
	  rm -f $$prg ; \
	done
	rm -f testlib.so
	rm -f ./*.o
	rm -f ./*.gcda
	rm -f ./*.gcov
	rm -f ./*.gcno
	rm -f tests/*.o
	rm libaccess.so
	rm libaccess.o

