#ifndef __UTIL
#define __UTIL

#include <stdbool.h>

extern char temp_buffer[256];

typedef struct t_buffer {
   char filename[50];
   char* data;
   int length;
   int cur_pos;
} t_buffer;

int read_file(char* filename, t_buffer* buffer);
int release_buffer(t_buffer* buffer);
int allocate_buffer(t_buffer* buffer, int len);
int write_file(char* filename, t_buffer* buffer,char* mode);

bool is_digit(char* val);
bool is_alnum(char* val);
bool is_alpha(char* val);
bool is_digit_all(char* val);
char* char_to_string(char val);

void db(char* v, int i);


#endif
