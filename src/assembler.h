#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "util.h"

// 100k chars max. make this dynamic later
#define MAX_ASM_LENGTH 20000

extern t_buffer main_block;
extern t_buffer proc_block;
extern t_buffer data_block;

void stamp(char val);
void assembler_init();
void part(char* str);
void as(char* str);
void comment(char* str);
void lbl(char* str);
void lbll(char* str);
void write(char* str, int step);
void str_doublet(char* a, char* b, char* c);
void str_triplet(char* a, char* b, char* c, char* d);

void set_current_block(t_buffer* buf);
void assembler_cleanup();

//void assembler_set_current_buffer()

void assembler_save(char* filename);



#endif
