#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char digit[] = "0123456789$%";

const char digit_all[] = "0123456789$%ABCDEFabcdef";

const char alpha[] = "abcdefghijklmnopqrstuvwxyz_";

const char alnum[] = "abcdefghijklmnopqrstuvwxyz_0123456789$%";

int read_file(char* filename, t_buffer* buffer) {
	FILE *f;
	f = fopen(filename, "r");
    if(f == NULL)
        return 1;

    fseek(f, 0L, SEEK_END);
    buffer->length = ftell(f);
    fseek(f, 0L, SEEK_SET);

    buffer->data = (char*)malloc(buffer->length);
    if(buffer->data == NULL)
        return 1;

    fread(buffer->data, sizeof(char), buffer->length, f);
    fclose(f);

		buffer->cur_pos = 0;

    return 0;
}

int write_file(char* filename, t_buffer* buffer, char* mode) {
	FILE *f;
	f = fopen(filename, mode);
  fwrite(buffer->data, sizeof(char), buffer->cur_pos, f);
  fclose(f);
  return 0;
}



int allocate_buffer(t_buffer* buffer, int len) {
    buffer->length = len;
    buffer->data = (char*)malloc(buffer->length);
    if(buffer->data == NULL)
        return 1;
		for (int i=0;i<len;i++)
			buffer->data[i]=0;

		buffer->cur_pos = 0;

    return 0;
}

int release_buffer(t_buffer* buffer) {
	if (buffer->length<1)
		return 1;
	free(buffer->data);
	buffer->data = NULL;
	return 0;
}

bool is_digit(char* val) {
	if (strstr(digit, val) != NULL) return true;
	return false;
}
bool is_digit_all(char* val) {
	if (strstr(digit_all, val) != NULL) return true;
	return false;
}
bool is_alpha(char* val) {
	if (strstr(alpha, val) != NULL) return true;
	return false;
}
bool is_alnum(char* val) {
	if (strstr(alnum, val) != NULL) return true;
	return false;
}


char temp_char[2];
char temp_buffer[256];

char* char_to_string(char val) {
	temp_char[0] = val;
	temp_char[1] = 0;
	return (char*)&temp_char;
}

// Debugging
void db(char* v, int i) {
  printf("%s :%d\n",v,i);
}
