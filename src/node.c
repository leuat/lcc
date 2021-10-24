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
  n->symtab = NULL;
  return n;
}

node* find_function(char* name) {
  node* t = functions;
  while (t!=NULL) {
    if (strcmp(t->token.str_value,name)==0)
      return t;
      t = t->right;
  }
	return NULL;
}

void define_function(node* s) {
  s->right = functions;
  functions = s;

}
