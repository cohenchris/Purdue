#
# CS252 - Shell Project
#
cc= gcc
ccFLAGS= -g -std=gnu11 -I"/homes/cs252/public/include"
WARNFLAGS= -Wall -Wextra -Werror -pedantic

LEX=lex -l
YACC=yacc -y -d -t --debug

EDIT_MODE_ON=yes

ifdef EDIT_MODE_ON
	EDIT_MODE_OBJECTS=tty_raw_mode.o read_line.o
endif

all: git-commit shell

lex.yy.o: shell.l 
	$(LEX) -o lex.yy.c shell.l
	$(cc) $(ccFLAGS) -c lex.yy.c

y.tab.o: shell.y
	$(YACC) -o y.tab.c shell.y
	$(cc) $(ccFLAGS) -c y.tab.c

command.o: command.c command.h
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c command.c

single_command.o: single_command.c single_command.h
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c single_command.c

shell.o: shell.c shell.h
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c shell.c

shell: y.tab.o lex.yy.o shell.o command.o single_command.o $(EDIT_MODE_OBJECTS)
		$(cc) $(ccFLAGS) $(WARNFLAGS) -o shell lex.yy.o y.tab.o shell.o command.o single_command.o $(EDIT_MODE_OBJECTS)

tty_raw_mode.o: tty_raw_mode.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c tty_raw_mode.c

read_line.o: read_line.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c read_line.c

.PHONEY: clean
clean:
	rm -f lex.yy.c y.tab.c y.tab.h shell *.o
	rm -f test_shell/out test_shell/out2
	rm -f test_shell/sh-in test_shell/sh-out
	rm -f test_shell/shell-in test_shell/shell-out
	rm -f test_shell/err1 test_shell/file-list

.PHONEY: git-commit
git-commit:
	@(git add *.c *.h shell.l shell.y; \
	 git add -f test_shell/testall.out; \
	 git commit -m "Latest build"; \
	 git push; \
	 echo "Succesfully recorded changes")

.PHONEY: submit_checkpoint
submit_part1:
	@git tag -fa v1.0 -m "Checkpoint"
	@git push -f --tags
	@echo "Part 1 submission complete"

.PHONEY: submit_final
submit_final:
	@git tag -fa v2.0 -m "Final"
	@git push -f --tags
	@echo "Final submission complete"

# DO NOT REMOVE: This ensures that shell.y/shell.l do not become
# implicity dependent on shell.c which results in one of the
# shell.l or shell.y rules overwriting shell.c and no one wants that
.SUFFIXES:
