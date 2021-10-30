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

  symbol* s = symbol_find(n->token.str_value);

  part(n->token.str_value);
  part(":\t");
  //printf("*** %s\n",n->left->token.str_value);

  if (n->token.is_pointer) {
    part("dq 0\n");
    return;
  }

  part(st->asm_val); // db, dw etc
  part("\t");
  bool ok = false;



  if (s->type->type.type == tt_char) {
    // do we have a string?
    if (n->token.large_string!=NULL) {
      if (n->token.is_string)
        part("\"");
      part(n->token.large_string);
      if (n->token.is_string)
        part("\",0");
      ok = true;
    }
    else {
      ok = true;
      t_token* t = n->token.next;
      while (t!=NULL) {
        part(t->str_value);
        t = t->next;
        if (t!=NULL) {
          part(",");
        }
      }
      
    }
  }
  if (!ok)
    part("0"); // Don't have initvalues
//  if (s->type->type.type == tt_char)
  //   db(n->token.large_string,0);
  part("\n");

}


void visit_call_proc(node* n) {

//  node* proc = find_function(n->token.str_value);
  node* param = n->left;
  int no_params = 0;
  while (param!=NULL) {
    //printf("JADDA %s\n ",param->token.str_value);
  //  db("ID ",param->type);
  
    codegen_visit(param);
    as("push rax");
    no_params++;
    param = param->right;
  }

  part("\tcall ");
  part(n->token.str_value);
  part("\n");
  for (int i=0;i<no_params;i++)
    as("pop rbx");
}


void visit_var_decl(node* n) {
//  printf("*** %s\n",n->token.str_value);
//  printf("*** %s\n",n->left->token.str_value);

  visit_define_general(n);

  // Proceed to next
  if (n->right!=NULL)
    visit_var_decl(n->right);
}

void codegen_load_variable(node* var) {
  if (var->token.is_reference) {
    part("\tmov rax, ");
    part(var->token.str_value);
    part("\n");
    return;
  }
  part("\tmov rax, [rel ");
  part(var->token.str_value);
  part("]\n");
}


void codegen_load_number(node* number) {
  part("\tmov rax, ");
 // itoa(var->token.ivalue,&temp_buff,10);
  sprintf(temp_buffer, "%d", number->token.ivalue);
  part(temp_buffer);
  part("\n");
}


void codegen_assign(node* n) {
//  db("N:",n->block->type);
  codegen_visit(n->block);
  part("\tmov [rel ");
  part(n->left->token.str_value);
  part("],rax\n");  
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

  if (node->type==nt_variable) {
    codegen_load_variable(node);
    return;
  }

 if (node->type==nt_number) {
    codegen_load_number(node);
    return;
  }

  if (node->type==nt_assign) {
    codegen_assign(node);
    return;
  }

}


void codegen_validate() {
  if (!has_main)
    raise_error("must contain 'main'.");
}
