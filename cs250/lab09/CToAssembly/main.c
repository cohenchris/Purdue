#include<stdio.h>
#include<malloc.h>


extern char* sub_string(char *, int, int);

int main() {
  int start_index, end_index;
  char * in_string = (char *)malloc(100 * sizeof(char *));
  char * out_string;

  printf("Enter a string: ");
  scanf("%[^\n]", in_string);
  printf("Enter the start index: ");
  scanf("%d", &start_index);
  printf("Enter the end index: ");
  scanf("%d", &end_index);

  out_string = sub_string(in_string, start_index, end_index);
  printf("The substring of the given string is '%s'\n", out_string);

  return 0;
}
