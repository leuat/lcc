#ifndef CODEGEN_H
#define CODEGEN_H

#include "util.h"
#include "assembler.h"
#include "node.h"

void codegen_init();

void codegen_cleanup();

void codegen_visit(node* root);

void codegen_validate();



#endif
