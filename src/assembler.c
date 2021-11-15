#include "assembler.h"
#include "error.h"
#include "util.h"

t_buffer* current_block;

t_buffer main_block;
t_buffer data_block;
t_buffer proc_block;

void set_current_block(t_buffer* buf) {
  current_block = buf;
}

void stamp(char val) {
  if (current_block==NULL) {
    raise_error("assembler_stamp error: current_block is null, should not happen.");
    return;
  }
  current_block->data[current_block->cur_pos]=val;
  current_block->cur_pos++;
}

void write(char* str, int step) {
  for (int i=0;i<step;i++)
    stamp('\t');
  int i=0;
  while (str[i]!=0) {
    stamp(str[i++]);
  }
  stamp('\n');

}

void part(char* str) {
  int i=0;
  while (str[i]!=0) {
    stamp(str[i++]);
  }
  
}

void assembler_init() {
  allocate_buffer(&main_block, MAX_ASM_LENGTH);
  allocate_buffer(&data_block, MAX_ASM_LENGTH);
  allocate_buffer(&proc_block, MAX_ASM_LENGTH);
  current_block = &main_block;
}

void assembler_cleanup() {
  release_buffer(&main_block);
  release_buffer(&data_block);
  release_buffer(&proc_block);
  current_block = NULL;
}

void lbl(char* str) {
  write(str,0);
}
void lbll(char* str) {
  part(".label ");
  part(str);
  part("\n");
}

void str_doublet(char* a, char* b, char* c) {
  part("\t");
  part(a);
  part("\t");
  part(b);
  part("\t");
  part(c);
  part("\n");
}
void str_triplet(char* a, char* b, char* c, char* d) {
  part("\t");
  part(a);
  part("\t");
  part(b);
  part("\t");
  part(c);
  part("\t");
  part(d);
  part("\n");
}

void as(char* str) {
  write(str,1);
}
void comment(char* str) {
  stamp(';');
  write(str,1);
}

void assembler_save(char* filename) {
  write_file(filename, &main_block,"w");
  write_file(filename, &proc_block,"a");
  write_file(filename, &data_block,"a");
}
