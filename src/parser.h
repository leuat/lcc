#ifndef PARSER_H
#define PARSER_H

#include "util.h"
#include "lexer.h"
#include "node.h"
#include "symboltable.h"


extern node* node_root;

void parse(t_buffer* buf);






#endif
