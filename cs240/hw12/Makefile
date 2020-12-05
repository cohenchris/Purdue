#
# Change the HW variable to match the homework
#
NUM	= 12
CLEAN = 

HW	= hw$(NUM)
CLASS	= /homes/cs240
PROJ	= $(CLASS)/public/homework/$(HW)
EXTRA = cube.obj f16.obj ico.obj mig23.obj oct.obj

CC	= gcc
CFLAGS	= -g -Wall -Werror -std=gnu99 -I$(CLASS)/public/include
LDFLAGS	= -L$(CLASS)/public/lib -lSDL -lX11 -lm

all:: $(HW)
all:: git

OBJS	=	object.o graphics.o main.o hw12.o

$(HW) : $(OBJS) Makefile $(EXTRA) git
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(HW)_debug	:	$(OBJS) Makefile $(EXTRA) git
	$(CC) -o $@ $(OBJS) $(LDFLAGS) -lsdlmd

object.o	:	object.c object.h graphics.h
graphics.o	:	graphics.c object.h
main.o	:	main.c object.h graphics.h
hw12.o	:	object.h graphics.h

Makefile : $(PROJ)/Makefile
	rm -f Makefile
	cp $(PROJ)/Makefile .

object.h : $(PROJ)/object.h
	rm -f object.h
	cp $(PROJ)/object.h .

object.c : $(PROJ)/object.c
	-rm -f object.c
	-cp $(PROJ)/object.c .

graphics.h : $(PROJ)/graphics.h
	rm -f graphics.h
	cp $(PROJ)/graphics.h .

graphics.c : $(PROJ)/graphics.c
	-rm -f graphics.c
	-cp $(PROJ)/graphics.c .

$main.c : $(PROJ)/main.c
	-rm -f main.c
	-cp $(PROJ)/main.c .

%.obj	:
	cp $(PROJ)/$@ .

#
# Extra targets to do interesting things...
#
clean :
	rm -f core *.o $(HW) $(HW)_debug $(CLEAN)

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
