#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "util.h"

// 100k chars max. make this dynamic later
#define MAX_ASM_LENGTH 100000

extern t_buffer main_block;
extern t_buffer data_block;

void assembler_init();
void as(char* str);
void comment(char* str);
void lbl(char* str);
void write(char* str, int step);

void set_current_block(t_buffer* buf);
void assembler_cleanup();

//void assembler_set_current_buffer()

void assembler_save(char* filename);



#endif
