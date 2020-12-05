# For gcc
CC= gcc
# For ANSI compilers
#CC= cc

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -fPIC -g

RM= /bin/rm -f
#--- You shouldn't have to edit anything else. ---

.c.o: 
	$(CC) -c $(CFLAGS) $<

all: jj

jj: jj.o util.o
	$(CC) $(CFLAGS) jj.o util.o -o ../cgi-bin/jj

clean:
	rm -f *.o ../cgi-bin/jj
