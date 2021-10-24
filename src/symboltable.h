#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <stdio.h>

#include "token.h"


typedef struct symbol_type {
    t_token type;
    struct symbol_type* next;

} symbol_type;


typedef struct symbol {
    t_token var;
    bool is_used;
    struct symbol_type* type;
    struct symbol* next;

} symbol;



symbol* symbol_table;
symbol_type* typespec_table;
symbol_type* reserved_words;


void initialize_symboltable();


symbol* create_symbol(t_token var, symbol_type* type);
symbol_type* create_symbol_type(t_token var);

void define_symbol(symbol* s);
void define_type(symbol_type* s);
void define_reserved_word(symbol_type* s);

symbol_type* find_type(char* type, symbol_type* parent);
symbol* symbol_find(char* name);

#endif
