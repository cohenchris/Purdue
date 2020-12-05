#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int myprintf(const char *, ...);
void printx(int, int);
void printd(int);
/*
int main() {
  myprintf("%d %d %d %d %d %d %d\n",52, -51, -3, 5, 74, 99, 101);
  myprintf("%d\n", 125);
}
*/
int myprintf(const char * str, ...) {
  void * arg = &str;
  arg += sizeof(char *);

  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == '%') {		//if it's a conversion character, (starts with %s), 
      if (str[i + 1] == 'c') {
	//just put single char with putchar
	putchar(*((char *)arg));
	arg += sizeof(char *);
      }
      else if (str[i + 1] == 's') {
      	//just print the string by looping
	char * string = *((char **)arg);
	for (int i = 0; i < strlen(string); i++) {
	  putchar(string[i]);
	}
	arg += sizeof(char **);
      }
      else if (str[i + 1] == 'x') {
	unsigned int hex = (*((unsigned int *)arg));
	printx(hex, 0);
	if (hex == 2147483648) {	//this if statement is an edge case
	  arg += sizeof(char *);	//for some reason, if the previous value entered was the largest negative number,
	}				//it messed up the next value being printed out

        arg += sizeof(char *);
      }
      else if (str[i + 1] == 'X') {
	unsigned int hex = (*((unsigned int *)arg));
	printx(hex, 1);
	if (hex == 2147483648) {	//this if statement is an edge case
	  arg += sizeof(char *);	//for some reason, if the previous value entered was the largest negative number,
	}				//it messed up the next value being printed out
        arg += sizeof(char *);
      }
      else if (str[i + 1] == 'd') {
        printd(*((signed int *)arg));
        arg += sizeof(char *);
      }
      else if (str[i + 1] == '%') {
        arg += sizeof(char *);
        putchar('%');
      }
      i++;
    }
    else {
      putchar(str[i]);
    }
  }
  return -1;
} //myprintf()
