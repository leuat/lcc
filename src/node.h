#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "symboltable.h"

typedef enum { nt_variable, nt_number, nt_func_decl, nt_func, nt_var_decl, nt_type_spec,
              nt_block, nt_root, nt_statement, nt_statement_list, nt_assign
            } node_type;


typedef struct node {
    node_type type;
    t_token token;
    struct node *left;
    struct node *right;
    struct node *center;
    struct node *block;
    struct symbol* symtab;
} node;


node* create_node(node_type nt, t_token tok);





#endif
