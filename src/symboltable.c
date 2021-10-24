#include "symboltable.h"


symbol_type* create_symbol_type(t_token var) {
  symbol_type* sym = malloc (sizeof (struct symbol_type));
  sym->type = var;
  sym->next = NULL;
  return sym;
}



symbol* create_symbol(t_token var,symbol_type* type) {
  symbol *sym = malloc (sizeof (struct symbol));
  sym->var = var;
  sym->type = type;
  sym->next = NULL;
  sym->is_used = false;
  return sym;
}


void define_symbol(symbol* s) {
  s->next = symbol_table;
  symbol_table = s;

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
  symbol_table = NULL;
  typespec_table = NULL;

  define_type( create_symbol_type(create_token(tt_id,"void",0)));
  define_type( create_symbol_type(create_token(tt_id,"int",0)));
  define_type( create_symbol_type(create_token(tt_id,"char",0)));


  define_reserved_word( create_symbol_type(create_token(tt_asm,"asm",0)));

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

symbol* symbol_find(char* name) {
  symbol* t = symbol_table;
  while (t!=NULL) {
    if (strcmp(t->var.str_value,name)==0)
      return t;
      t = t->next;
  }
	return NULL;
}
