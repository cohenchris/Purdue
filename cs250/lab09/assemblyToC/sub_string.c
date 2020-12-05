#include<stdio.h>
#include<malloc.h>


char * sub_string(char *in_string, int start_index, int end_index) {
  char * out_string = (char *)malloc(100 * sizeof(char));
  
  for (int i = start_index - 1; i < end_index; i++) {
    out_string[i - (start_index - 1)] = in_string[i];
  }
  out_string[end_index] =  '\0';

  return out_string;
} //sub_string()
