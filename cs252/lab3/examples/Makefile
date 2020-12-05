#
# CS252 - Shell Project Examples
#
#Use GNU compiler
cc= gcc
ccFLAGS= -g -std=gnu11
WARNFLAGS= -Wall -Wextra -pedantic

all: new_process ls_grep ctrl_c keyboard_example read_line_example

new_process: new_process.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -o new_process new_process.c

ls_grep: ls_grep.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -o ls_grep ls_grep.c

ctrl_c: ctrl_c.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -o ctrl_c ctrl_c.c

tty_raw_mode.o: ../tty_raw_mode.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c ../tty_raw_mode.c

keyboard_example: keyboard_example.c tty_raw_mode.o
	$(cc) $(ccFLAGS) $(WARNFLAGS) -o keyboard_example keyboard_example.c tty_raw_mode.o

read_line.o: ../read_line.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c ../read_line.c

read_line_example: read_line_example.c tty_raw_mode.o read_line.o
	$(cc) $(ccFLAGS) $(WARNFLAGS) -o read_line_example read_line_example.c tty_raw_mode.o read_line.o

.PHONY: clean
clean:
	rm -f new_process ctrl_c ls_grep keyboard_example read_line_example *.o

