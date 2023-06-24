#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "colours.h"

#define N_TOKEN_TYPES 2
#define N_ATTRIBUTE_TYPES 2
#define TML_TAG_NAME_LEN 21
#define TML_ATTRIBUTE_NAME_LEN 21
#define TML_ATTRIBUTE_VALUE_LEN 21

enum ast_node_type_e
{
  TML_NODE_NONE = 0u,        // 0000 0000
  TML_NODE_ROOT = (1 << 0u), // 0000 0001
  TML_NODE_TEXT = (1 << 1u), // 0000 0010
};

enum ast_attribute_type_e
{
  TML_ATTRIBUTE_NULL = 0u,                // 0000 0000
  // fg=...
  TML_ATTRIBUTE_FOREGROUND = (1 << 0u),   // 0000 0001
  // bg=...
  TML_ATTRIBUTE_BACKGROUND = (1 << 1u),   // 0000 0010
};

struct ast_attribute_pair_t
{
  enum ast_attribute_type_e type;
  char value[TML_ATTRIBUTE_VALUE_LEN];
};

struct ast_t
{
  enum ast_node_type_e type;

  struct ast_t* parent;

  uint64_t allowed_children_nodes; // a bitmap composed of ast_node_type_e
  enum ast_node_type_e* children_types;
  struct ast_t* children;
  size_t n_children;
  size_t children_capacity;

  struct ast_attribute_pair_t* attributes;
  size_t n_attributes;
  size_t attribute_capacity;
};

struct ast_t*
ast_create(
  const enum ast_node_type_e type,
  struct ast_t* parent);

bool
ast_add_attribute(
  struct ast_t* node,
  enum ast_attribute_type_e type,
  const char* const value);

void
ast_free(
  struct ast_t** root);

#endif
