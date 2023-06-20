#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdlib.h>

#include "colours.h"

// forward declarations
struct text_node_t;

enum ast_node_type_e
{
  TML_NONE = -1,
  TML_TEXT = 0,
  TML_ROOT
};

union ast_root_children_u
{
  struct text_node_t* node_text;
};

struct ast_t
{
  enum ast_node_type_e* children_types;
  union ast_root_children_u* children;
  size_t n_children;
  size_t capacity;
  enum colour_e bg_colour;
};

struct ast_t*
ast_create(
  const char* const bg_colour);

void
ast_free(
  struct ast_t** root);

#endif
