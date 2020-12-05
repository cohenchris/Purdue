all: main

main: pwm.c

	gcc -std=c99 -o pwm pwm.c -lwiringPi -lpthread

clean:
	rm -r *.o
