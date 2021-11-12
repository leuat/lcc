#include "codegen.h"
#include <string.h>
#include "assembler.h"
#include "error.h"
bool has_main = false;


int current_register = 0;

symbol* current_symbol = NULL;


char* current_register_str[4]; 


void push_reg() {
  sprintf(current_register_str, "_r%d", current_register++);
}

void pop_reg() {
  if (current_register==0) 
    raise_error("Cannot pop register back from zero.");
  sprintf(current_register_str, "_r%d", --current_register);
}





void codegen_cleanup() {
  assembler_cleanup();
}
void codegen_init() {
  assembler_init();
  set_current_block(&main_block);
  comment("Hello world!");
  lbl(".tripe\tuint8:1");
//  as("section\t.text");
  //lbl("_start:");
  if (current_system==system_c64) {
    lbl(".code uint16:0x801 run");
    as(".uint8 0b  08 0a 00 9e 20 32  30 36 34  00  00  00  ff");
  }

  set_current_block(&data_block);
  lbl(".data uint8:0");
  set_current_block(&main_block);
  if (current_system==system_c64) {
    lbl(".code uint16:0x810 block1");
  }

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
    part(".label\t");
    part(n->token.str_value);
    part("\n");
  }
  visit_all_right(n->block->right);
  if (!is_main) {
    as("return");
  }

}


void visit_define_general(node*n) {
  symbol_type* st = find_type(n->left->token.str_value, typespec_table);
  if (st==NULL)
    raise_error("codegen visit_var_decl NULL should not happen!");

  symbol* s = symbol_find(n->token.str_value);
  if (n->token.is_pointer) {
      part("\tdeclptr ");
      part(n->token.str_value);
      part("\tuint16:0\n");

    return;
  }
  else
    part("\tdecl ");

  part(n->token.str_value);
  part("\t");
  //printf("*** %s\n",n->left->token.str_value);


  part(st->asm_val); // db, dw etc
  part(":");
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


bool is_pure(node* n) {
  return (n->type == nt_number || (n->type == nt_variable));
}

void tripe_val(node* n2, char* v) {

  node* n = n2;
//  if (!is_pure(n)) {
  if (!is_pure(n) || (n->center!=NULL && n->value!=-1)) {
    codegen_visit(n);
    pop_reg();
    strcpy(v,current_register_str);
    return;
  }
   

  if (n->type==nt_number) {
//    db("TYP " ,n->token.type);
    sprintf(v, "%s:%d", current_symbol->type->asm_val, n->token.ivalue);
  }
  if (n->type==nt_variable) {
    strcpy(v,n->token.str_value);
  }
}


void doublet(node* a, node* b, char* cmd)
{
      char l[128] = "";
    char r[128] = "";
    char idx[128] = "";


    tripe_val(a,l);
    tripe_val(b,r);

    part("\t");
    part(cmd);
    part("\t");
    part(l);
    part("\t");
    part(r);
    part("\n");

}

void triplet(node* a, node* i, node* b, char* cmd)
{
    char l[128] = "";
    char r[128] = "";
    char idx[128] = "";

    push_reg();
    tripe_val(i,idx);
    pop_reg();
    tripe_val(a,l);
    tripe_val(b,r);

    part("\t");
    part(cmd);
    part("\t");
    part(l);
    part("\t");
    part(idx);
    part("\t");
    part(r);
    part("\n");

}




void codegen_load_variable(node* var) {

  if (var->center!=NULL) { // has index
      char iddx[128];
      char r0[128];
      push_reg();
      tripe_val(var->center, iddx);
      part("\tload_p ");
      part(current_register_str);
      part("\t");
      part(var->token.str_value);
      part("\t");
      part(iddx);
      part("\n");
      return;
    }

  //raise_error("codegen_load_variable should be called");
/*
  if (var->block!=NULL) {
    tripe_val(var->block,l);
    if (var->token.is_pointer) {
//      doublet()
    }
  }
*/
}




void codegen_load_number(node* number) {
  sprintf(temp_buffer, "%d", number->token.ivalue);
  part(temp_buffer);
}

void codegen_assign(node* n) {
  //if (is_pure(n->block)) {

  current_symbol = symbol_find(n->left->token.str_value);

//     doublet(n->left, n->block, "mov");

//  db(n->left->token.str_value,0);
    n->left->value = -1; // flag as STORE type
    if (n->left->center==NULL)
     doublet(n->left, n->block, "mov");
    else 
     triplet(n->left, n->left->center,n->block, "store_p");

//     doublet(n->left, n->block, "mov");

} 

void codegen_binop(node* node) {
  char cmd[10];
  push_reg();
  if (node->token.type == tt_plus)  
    sprintf(cmd,"add %s ",current_register_str);
  if (node->token.type == tt_minus)  
    sprintf(cmd,"sub %s ",current_register_str);
  if (node->token.type == tt_mul)  
    sprintf(cmd,"mulu %s ",current_register_str);
  if (node->token.type == tt_div)  
    sprintf(cmd,"divu %s ",current_register_str);

    doublet(node->left, node->block, cmd);
}


void codegen_visit(node* node)
{
  if (node->type == nt_root) {
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

  if (node->type==nt_binop) {
    codegen_binop(node);
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
