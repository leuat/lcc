#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <stdio.h>

#include "token.h"


typedef struct symbol_type {
    t_token type;
    char asm_val[32];

    struct symbol_type* next;

} symbol_type;


typedef struct symbol {
    t_token var;
    bool is_used;
    struct symbol_type* type;
    struct symbol* next;

} symbol;


typedef struct t_symbol_table {
    struct symbol* symbols;
    struct symbol_table* child, *parent;
} t_symbol_table;



t_symbol_table* root_symbol_table;
t_symbol_table* current_symbol_table;
symbol_type* typespec_table;
symbol_type* reserved_words;


void initialize_symboltable();


symbol* create_symbol(t_token var, symbol_type* type);
symbol_type* create_symbol_type(t_token var, char* asm_val);

void define_symbol(symbol* s);
void define_type(symbol_type* s);
void define_reserved_word(symbol_type* s);

void push_symbol_table();

void pop_symbol_table();

t_symbol_table* create_symbol_table();

symbol_type* find_type(char* type, symbol_type* parent);
symbol* symbol_find(char* name);

#endif
