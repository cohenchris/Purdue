#include <stdio.h>
#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS

void dc_write_handle(void *, int, const char *, const char *);
void dc_read_handle(void *, int, const char *, const char *);
#define str_indir(X) #X
#define str(X) str_indir(X)

#define DC_WRITE(X) dc_write_handle(&X, sizeof(X), #X, __FILE__ ":" str(__LINE__))
#define DC_READ(X) dc_read_handle(&X, sizeof(X), #X,__FILE__ ":" str(__LINE__))

#endif
