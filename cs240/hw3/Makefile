#
# Change the HW variable to match the homework
#
NUM	= 3

HW	= hw$(NUM)
CLASS	= /homes/cs240
PROJ	= $(CLASS)/public/homework/$(HW)

CC	= gcc
CFLAGS	= -g -Wall -Werror -std=c99 -I$(CLASS)/public/include
LDFLAGS	= -L$(CLASS)/public/lib -lmd -lm

all:: $(HW)_main
all:: $(HW)_test
all:: $(HW)_view
all:: git

$(HW)_main : $(HW)_main.o $(HW).o git Makefile
	$(CC) -o $(HW)_main $(HW)_main.o $(HW).o $(LDFLAGS)

$(HW)_test : $(HW)_test.o $(HW).o git Makefile
	$(CC) -o $(HW)_test $(HW)_test.o $(HW).o $(LDFLAGS)

$(HW)_view : $(HW)_view.o $(HW).o view.o git Makefile
	$(CC) -o $(HW)_view $(HW)_view.o $(HW).o view.o -L/usr/X11R6/lib -lX11 -lm

$(HW).o : Makefile $(HW).h
$(HW)_main.o : Makefile $(HW).h

$(HW)_test.o : $(PROJ)/$(HW)_test.o
	rm -f $(HW)_test.o
	cp $(PROJ)/$(HW)_test.o .

$(HW)_view.o : $(PROJ)/$(HW)_view.o
	rm -f $(HW)_view.o
	cp $(PROJ)/$(HW)_view.o .

view.o : $(PROJ)/view.o
	rm -f view.o
	cp $(PROJ)/view.o .

Makefile : $(PROJ)/Makefile
	rm -f Makefile
	cp $(PROJ)/Makefile .

$(HW).h : $(PROJ)/$(HW).h
	rm -f $(HW).h
	cp $(PROJ)/$(HW).h .

$(HW)_main.c : $(PROJ)/$(HW)_main.c
	-rm -f $@
	-cp $(PROJ)/$(HW)_main.c .

#
# Extra targets to do interesting things...
#
clean :
	rm -f core *.o $(HW)_main $(HW)_test $(HW)_view

git:
		git checkout master >> .local.git.out || echo
		git add *.c *.h  >> .local.git.out || echo
		git commit -a -m "Commit homework $(HW)" >> .local.git.out || echo
		git push origin master

submit:
		git checkout master >> .local.git.out || echo
		git add *.c *.h  >> .local.git.out || echo
		git commit --allow-empty -a -m "Submission for homework $(HW)" >> .local.git.out || echo
		git push origin master

verify:
		~cs240/bin/sub-verify $(HW)
