#include "codegen.h"
#include <string.h>
#include "assembler.h"
#include "error.h"
bool has_main = false;

#define N 64

int current_register = 0;

symbol* current_symbol = NULL;

int current_label=0;
char current_label_str[N];
char cur_default_int_type[N] = "uint8";


char* get_int(int i) {
  sprintf(temp_buffer,"%s:%d",cur_default_int_type,i);
  return temp_buffer;
}


void push_label() {
  sprintf(current_label_str, "label_%d", current_label++);
}

void pop_label() {
  if (current_label==0) 
    raise_error("Cannot pop label back from zero.");
  sprintf(current_label_str, "_r%d", --current_label);
}


char current_register_str[4]; 


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
      str_doublet("declptr",n->token.str_value,"uint16:0");

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
    if (n->token.ivalue == plusplus_post) 
        str_triplet("add",n->token.str_value,n->token.str_value,get_int(1));
    if (n->token.ivalue == minusminus_post) 
        str_triplet("sub",n->token.str_value,n->token.str_value,get_int(1));

  }
}


void doublet(node* a, node* b, char* cmd)
{
      char l[N] = "";
    char r[N] = "";
    char idx[N] = "";


    tripe_val(a,l);
    tripe_val(b,r);

    str_doublet(cmd,l,r);

}

void triplet(node* a, node* i, node* b, char* cmd)
{
    char l[N] = "";
    char r[N] = "";
    char idx[N] = "";

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

  if (var->token.ivalue!=0) { // ++ or --
    if (var->center!=NULL) raise_error("lcc does not yet support ++ -- on arrays");
    if (var->token.ivalue == plusplus_post) 
        str_triplet("add",var->token.str_value,var->token.str_value,get_int(1));
    if (var->token.ivalue == minusminus_post) 
        str_triplet("sub",var->token.str_value,var->token.str_value,get_int(1));

  }


  if (var->center!=NULL) { // has index
      char iddx[N];
      char r0[N];
      push_reg();
      tripe_val(var->center, iddx);
      str_triplet("load_p",current_register_str,var->token.str_value,iddx);
      return;
    }
 
}




void codegen_load_number(node* number) {
  sprintf(temp_buffer, "%d", number->token.ivalue);
  part(temp_buffer);
}

void codegen_assign(node* n) {
  //if (is_pure(n->block)) {

  current_symbol = symbol_find(n->left->token.str_value);

    n->left->value = -1; // flag as STORE type
    if (n->left->center==NULL)
     doublet(n->left, n->block, "mov");
    else 
     triplet(n->left, n->left->center,n->block, "store_p");

//     doublet(n->left, n->block, "mov");

} 

char* get_binop(node* n) {
  if (n->token.type == tt_plus)  
    return "add";
  if (n->token.type == tt_minus)  
    return "sub";
  if (n->token.type == tt_mul)  
    return "mulu";
  if (n->token.type == tt_div)  
    return "divu";

  return "[unknown binop]";
}
/*
bool are_same_vars(node*a , node* b) {
  if (!(a->type == nt_variable && b->type==nt_variable))
    return false; 

//  db(a->token.str_value,0);
//  db(b->token.str_value,0);

  if (strcmp(a->token.str_value,b->token.str_value)==0)
    return true;
    
  return false;
}
*/
void codegen_binop(node* node) {
  char cmd[10];

/*  if (node->block->left!=NULL)
  if (are_same_vars(node->left,node->block->left)) {
    sprintf(cmd,"%s %s ",get_binop(node),node->left->token.str_value);
    doublet(node->left, node->block, cmd);
    db("ARE SAME ",0);
    return;
  }
*/


  push_reg();
  sprintf(cmd,"%s %s ",get_binop(node),current_register_str);
  doublet(node->left, node->block, cmd);
}


char* get_branch(enum_token_type t, bool inv) {
  if (!inv) {
  if (t==tt_gt){
    return "bgtu";
  }
  if (t==tt_lt){
    return "bltu";
  }
  if (t==tt_equals){
    return "bne";
  }
  if (t==tt_not_equals){
    return "beq";
  }
  }
  if (inv) {
    if (t==tt_lt){
      return "bgtu";
    }
    if (t==tt_gt){
      return "bltu";
    }
    if (t==tt_not_equals){
      return "bne";
    }
    if (t==tt_equals){
      return "beq";
    }
      
  }
  return "[uknown branch]";
}



void visit_binary_clause(node* n, char* block, char* els, char* done, bool inv) {
    push_reg();
    push_label();
    doublet(n->left, n->block,"cmp");
    str_doublet(get_branch(n->token.type,inv),els,"");

}


void visit_conditional(node* n) {
  char lbl_done[N];
  char lbl_else[N];
  char lbl_block[N];

  push_label();
  strcpy(lbl_done,current_label_str);
  push_label();
  strcpy(lbl_else,current_label_str);
  push_label();
  strcpy(lbl_block,current_label_str);

  visit_binary_clause(n->center, lbl_block, lbl_else, lbl_done, false);
  lbll(lbl_block);
  codegen_visit(n->block);

  if (n->left != NULL) // else block
   str_doublet("jump",lbl_done,"");

  lbll(lbl_else);

  if (n->left != NULL)
    codegen_visit(n->left);

  lbll(lbl_done);
}




void visit_binary_clause_expr(node* n) {
    push_reg();
    push_label();
    str_doublet("mov",current_register_str, get_int(0));

    doublet(n->left, n->block,"cmp");
    push_label();
    str_doublet(get_branch(n->token.type,false),current_label_str,"");
    str_doublet("mov",current_register_str, get_int(1));

    lbll(current_label_str);

   // d(n->left, n->block,"bne");

}


void visit_for_loop(node* n) {
  // For loop!
    comment("for loop");
    push_label();
    char lbl[N];
    strcpy(lbl,current_label_str);
    codegen_visit(n->left); //  i = 0;
    lbll(lbl); // define top label

    codegen_visit(n->block); // Main block


    codegen_visit(n->extra); //  i++;

//    codegen_visit(n->center); //  i<10;
    visit_binary_clause(n->center, "", lbl, "", true);
    pop_reg();

}

void visit_while_loop(node* n) {
  // For loop!
    comment("while loop");
    push_label();
    char lbl[N];
    strcpy(lbl,current_label_str);
    lbll(lbl); // define top label
    codegen_visit(n->block); //  i = 0;

    visit_binary_clause(n->left, "", lbl, "", true);
    pop_reg();

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
  if (node->type == nt_block) {
    visit_block(node);
    return;
  }
  if (node->type == nt_for) {
    visit_for_loop(node);
    return;
  }
  if (node->type == nt_while) {
    visit_while_loop(node);
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
  if (node->type==nt_clause_expr) {
    visit_binary_clause_expr(node);
    return;
  }
  if (node->type==nt_conditional) {
    visit_conditional(node);
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
