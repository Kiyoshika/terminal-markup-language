#include "ast.h"

struct ast_t*
ast_create(
  const enum ast_node_type_e type,
  struct ast_t* parent)
{
  struct ast_t* node = malloc(sizeof(*node));
  if (!node)
    return NULL;

  node->parent = parent;

  node->type = type; 

  switch (node->type)
  {
    case TML_NODE_ROOT:
      node->allowed_children_nodes = TML_NODE_TEXT;
      break;
    case TML_NODE_TEXT:
      node->allowed_children_nodes = TML_NODE_NONE;
      break;
    case TML_NODE_NONE:
      break;
  }

  node->n_children = 0;
  node->children_capacity = 1;
  node->children_types = calloc(node->children_capacity, sizeof(*node->children_types));
  node->children = calloc(node->children_capacity, sizeof(*node->children));

  node->n_attributes = 0;
  node->attribute_capacity = 1;
  node->attributes = calloc(node->attribute_capacity, sizeof(*node->attributes));

  if (!node->children_types || !node->children || !node->attributes)
  {
    free(node->children_types);
    free(node->children);
    free(node->attributes);
    free(node);
    return NULL;
  }

  return node;
}

bool
ast_add_attribute(
  struct ast_t* node,
  enum ast_attribute_type_e type,
  const char* const value)
{
  struct ast_attribute_pair_t attribute = {
    .type = type,
    .value = {0}
  };
  strncat(attribute.value, value, TML_ATTRIBUTE_VALUE_LEN);

  memcpy(&node->attributes[node->n_attributes++], &attribute, sizeof(attribute));
  
  if (node->n_attributes == node->attribute_capacity)
  {
    size_t new_capacity = node->attribute_capacity *= 2;
    void* alloc = realloc(node->attributes, new_capacity * sizeof(*node->attributes));
    if (!alloc)
      return false;
    node->attributes = alloc;
    node->attribute_capacity = new_capacity;
  }

  return true;
}

void
ast_free(
  struct ast_t** root)
{
  if (!*root)
    return;

  free((*root)->children);
  (*root)->children = NULL;

  free((*root)->children_types);
  (*root)->children_types = NULL;

  free(*root);
  *root = NULL;
}
