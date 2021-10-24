#ifndef __UTIL
#define __UTIL

#include <stdbool.h>

typedef struct t_buffer {
   char filename[50];
   char* data;
   int length;
} t_buffer;

int read_file(char* filename, t_buffer* buffer);
int release_buffer(t_buffer* buffer);


#endif