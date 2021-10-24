#include "util.h"
#include <stdio.h>
#include <stdlib.h>

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
 
    return 0;
}

int release_buffer(t_buffer* buffer) {
	if (buffer->length<1)
		return 1;
	free(buffer->data);
	buffer->data = NULL;
	return 0;
}

