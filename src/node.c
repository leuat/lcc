#include "node.h"
#include <string.h>

node* create_node(node_type nt, t_token tok) {
  node *n = malloc (sizeof (struct node));
  n->type = nt;
  n->token = tok;
  n->right = NULL;
  n->left = NULL;
  n->center = NULL;
  n->block = NULL;
  n->extra = NULL;
  n->symtab = NULL;
  n->value = 0;
  return n;
}

node* find_function(char* name) {
  node* t = functions;
  while (t!=NULL) {
//    printf("looking for function: %s\n",t->token.str_value);
    if (strcmp(t->token.str_value,name)==0)
      return t;

    t = t->left;
  }
	return NULL;
}

void define_function(node* s) {
//  printf("Defining function: %s\n",s->token.str_value);
  s->left = functions;
  functions = s;
}
