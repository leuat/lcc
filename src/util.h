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


bool is_digit(char* val);
bool is_alnum(char* val);
bool is_alpha(char* val);
bool is_digit_all(char* val);
char* char_to_string(char val);

#endif
