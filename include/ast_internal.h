#ifndef AST_INTERNAL_H
#define AST_INTERNAL_H

#include <stddef.h>

//forward declarations
struct ast_t;
struct ast_attributes_t;
struct iarray_t;

void
ast_render_text(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y);

void
ast_render_space(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y);

void
ast_render_input(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y);
#endif
