#include "codegen.h"


void codegen_cleanup() {
  assembler_cleanup();
}
void codegen_init() {
  assembler_init();
  set_current_block(&main_block);
  comment("Hello world!");
  as("global\t_start");
  as("section\t.text");
  lbl("_start:");

  set_current_block(&data_block);
  as("section\t.data");
  set_current_block(&main_block);

}

void codegen_visit(node* node);


void visit_all_right(node* node) {
  while (node!=NULL) {
    db("right",0);
      codegen_visit(node);
      node = node->right;
  }
}


void codegen_visit(node* node)
{
  if (node->type == nt_root) {
    db(".. ROOOT!",0);
    visit_all_right(node->right);
    return;
  }

}
