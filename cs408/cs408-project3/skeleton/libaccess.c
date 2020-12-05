#include "libaccess.h"
#include <stdio.h>
extern void dc_write_handle(void * addr, int access_size, const char *addr_name, const char *src_loc){
  printf("Calling write handler\n");

}

extern  void dc_read_handle(void * addr, int access_size, const char*addr_name, const char * src_loc){
  printf("Calling read handler\n");
}