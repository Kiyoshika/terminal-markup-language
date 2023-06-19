#include "ast.h"

struct ast_t*
ast_create(
  const char* const bg_colour)
{
  struct ast_t* root = malloc(sizeof(*root));
  if (!root)
    return NULL;

  // default colour is black
  if (!bg_colour)
    root->bg_colour = TML_BLACK;
  else
  {
    enum colour_e colour = tml_get_colour(bg_colour);
    if (colour == TML_UNKNOWN)
    {
      free(root);
      return NULL;
    }
    root->bg_colour = colour;
  }

  root->n_children = 0;
  root->capacity = 16;
  root->children_types = calloc(root->capacity, sizeof(*root->children_types));
  root->children = calloc(root->capacity, sizeof(*root->children));

  if (!root->children_types || !root->children)
  {
    free(root);
    free(root->children_types);
    free(root->children);
    return NULL;
  }

  return root;
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
