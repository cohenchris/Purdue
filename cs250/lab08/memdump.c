#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void memdump(char * p, int len) {
  int i;
  unsigned char buff[17];
  unsigned char *pc = (unsigned char *)p;

  for (int i = 0; i < len; i++) {
    if ((i%16) == 0) {
      if (i != 0)
	printf(" %s\n", buff);

      printf("0x%x:", &pc[i]);
    }

    printf(" %02x", pc[i]);

    if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
      buff[i % 16] = '.';
    }
    else {
      buff[i % 16] = pc[i];
    }

    buff[(i % 16) + 1] = '\0';
  }
  printf(" %s\n", buff);
}

struct X {
  char a;
  int i;
  char b;
  int * p;
};

struct List {
  char * str;
  struct List * next;
};

int main() {
  double y = 10;
  char str[20];
  int a = 33;
  int b = -44;
  struct X x;
  struct List * head;

  x.a = 'B';
  x.i = 21;
  x.b = '0';
  x.p = &x.i;
  strcpy(str, "Summer 2018\n");
  printf("&x=0x%x\n", &x.a);
  printf("&y=0x%x\n", &y);

  memdump((char *) &x.a, 64);
  head = (struct List *) malloc(sizeof(struct List));
  head->str = strdup("Greetings ");
  printf("\nsizeof head = %d\n", sizeof(head));
  head->next = (struct List *) malloc(sizeof(struct List));
  head->next->str = strdup("from ");
  printf("\nsizeof head->next = %d\n", sizeof(head->next));
  head->next->next = (struct List *) malloc(sizeof(struct List));
  head->next->next->str = strdup("Raspberry Pi");
  head->next->next->next = NULL;
  printf("sizeof head->next->next = %d\n", sizeof(head->next->next));
  printf("head=0x%x\n", head);
  memdump((char*) head, 128);
}


