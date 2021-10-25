#include "codegen.h"
#include <string.h>
#include "assembler.h"
#include "error.h"
bool has_main = false;


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
  if (node!=NULL)
    codegen_visit(node);

  if (node->right!=NULL)
    visit_all_right(node->right);
/*  while (node!=NULL) {
    db(node->token.str_value,0);
      codegen_visit(node);
      node = node->right;
  }
  */
}

void visit_all_center(node* node) {
  while (node!=NULL) {
//    db("right",0);
      codegen_visit(node);
      node = node->center;
  }
}

void visit_block(node* n) {
    visit_all_right(n->right);
}



void visit_proc_decl(node* n) {
  bool is_main = false;
//  printf("%s\n",n->token.str_value);

  if (strcmp(n->token.str_value,"main")==0) {
    has_main = true;
    set_current_block(&main_block);
    is_main = true;
  }
  if (!is_main) {
    part(n->token.str_value);
    part(":\n");
  }
  visit_all_right(n->block->right);
  if (!is_main) {
    as("ret");
  }

}


void visit_define_general(node*n) {
  symbol_type* st = find_type(n->left->token.str_value, typespec_table);
  if (st==NULL)
    raise_error("codegen visit_var_decl NULL should not happen!");

  part(n->token.str_value);
  part(":\t");
  //printf("*** %s\n",n->left->token.str_value);
  part(st->asm_val); // db, dw etc
  part("\t");
  part("0"); // Don't have initvalues yet!
  part("\n");

}
void visit_call_proc(node* n) {
  // No parameters yet
  part("call ");
  part(n->token.str_value);
  part("\n");
}


void visit_var_decl(node* n) {
//  printf("*** %s\n",n->token.str_value);
//  printf("*** %s\n",n->left->token.str_value);

  visit_define_general(n);

  // Proceed to next
  if (n->right!=NULL)
    visit_var_decl(n->right);
}



void codegen_visit(node* node)
{
//  printf("%d\n",node->type);
  if (node->type == nt_root) {
  //  db("ROOOT!",0);
    visit_all_center(node->center);
    return;
  }
  if (node->type == nt_var_decl) {
    set_current_block(&data_block);
    visit_var_decl(node);
    return;
  }
  if (node->type == nt_func_decl) {
    set_current_block(&proc_block);
    visit_proc_decl(node);
    return;
  }
  if (node->type==nt_statement_list) {
//    db("Statement list!",0);
  }
  if (node->type==nt_assign) {
    db("Assignment!",0);
  }
  // Inline assembler
  if (node->type==nt_asm) {
    int i=0;
    while (node->token.large_string[i]!=0) {
      stamp(node->token.large_string[i++]);
    }
    return;
  }

  if (node->type==nt_func) {
    visit_call_proc(node);
    return;
  }

}


void codegen_validate() {
  if (!has_main)
    raise_error("must contain 'main'.");
}
