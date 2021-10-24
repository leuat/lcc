#include "node.h"

node* create_node(node_type nt, t_token tok) {
  node *n = malloc (sizeof (struct node));
  n->type = nt;
  n->token = tok;
  n->right = NULL;
  n->left = NULL;
  n->center = NULL;
  n->block = NULL;
  return n;
}
