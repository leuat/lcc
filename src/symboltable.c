#include "symboltable.h"
#include <string.h>

symbol_type* create_symbol_type(t_token var, char* asm_val) {
  symbol_type* sym = malloc (sizeof (struct symbol_type));
  strcpy(sym->asm_val,asm_val);
  //sym->asm_val = asm_val;
  sym->type = var;
  sym->next = NULL;
  return sym;
}


t_symbol_table* create_symbol_table() {
  t_symbol_table *st = malloc (sizeof (struct t_symbol_table));
  st->child = NULL;
  st->parent = NULL;
  st->symbols = NULL;
  return st;
}

symbol* create_symbol(t_token var,symbol_type* type) {
  symbol *sym = malloc (sizeof (struct symbol));
  sym->var = var;
  sym->type = type;
  sym->next = NULL;
  sym->is_used = false;
  return sym;
}

void push_symbol_table() {
  t_symbol_table* nt = create_symbol_table();
  current_symbol_table->child = nt;
  nt->parent = current_symbol_table;
  current_symbol_table = nt;
}

void pop_symbol_table() {
  current_symbol_table = current_symbol_table->parent;
  current_symbol_table->child = NULL;

}

void define_symbol(symbol* s) {
  s->next = current_symbol_table->symbols;
  current_symbol_table->symbols = s;

}

void define_type(symbol_type* s) {
  s->next = typespec_table;
  typespec_table = s;

}

void define_reserved_word(symbol_type* s) {
  s->next = reserved_words;
  reserved_words = s;

}


void initialize_symboltable() {
  root_symbol_table = create_symbol_table();
  current_symbol_table = root_symbol_table;
  typespec_table = NULL;

  define_type( create_symbol_type(create_token(tt_id,"void",0),""));
  define_type( create_symbol_type(create_token(tt_int,"int",0),"uint16"));
  define_type( create_symbol_type(create_token(tt_char,"char",0),"uint8"));
  define_type( create_symbol_type(create_token(tt_long,"long",0),"uint32"));
  define_type( create_symbol_type(create_token(tt_int64,"int64",0),"uint64"));

  define_type( create_symbol_type(create_token(tt_pointer,"pointer",0),"uint16"));


  define_reserved_word( create_symbol_type(create_token(tt_asm,"asm",0),""));
  define_reserved_word( create_symbol_type(create_token(tt_if,"if",0),""));
  define_reserved_word( create_symbol_type(create_token(tt_else,"else",0),""));
  define_reserved_word( create_symbol_type(create_token(tt_for,"for",0),""));

}


symbol_type* find_type(char* type, symbol_type* parent) {
  symbol_type* t = parent;
  while (t!=NULL) {
    if (strcmp(t->type.str_value,type)==0)
      return t;
      t = t->next;
  }

	return NULL;
}


symbol* symbol_find_recursive(t_symbol_table* symtab, char* name) {
  if (symtab==NULL)
  	return NULL;
  symbol* t = symtab->symbols;
  while (t!=NULL) {
    if (strcmp(t->var.str_value,name)==0)
      return t;
      t = t->next;
  }
  return symbol_find_recursive(symtab->parent, name);

}


symbol* symbol_find(char* name) {
  return symbol_find_recursive(current_symbol_table,name);
}
