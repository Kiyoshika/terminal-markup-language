#ifndef IARRAY_H
#define IARRAY_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* iarray = interactive array */
/* an array of all interactive elements (input, button, etc.) */
/* keep track of the position and size (length) so we can check
   mouse events against it */
 
// forward declarations
struct ast_t;

struct iarray_item_t
{
  struct ast_t* node; // shared pointer to the node this item refers to (DOES NOT get free'd with array)
  size_t x;
  size_t y;
  size_t width;
};

struct iarray_t
{
  struct iarray_item_t* items;
  size_t length;
  size_t capacity;
};

struct iarray_t*
iarray_create();

bool
iarray_add(
  struct iarray_t* const array,
  struct ast_t* node,
  size_t x,
  size_t y,
  size_t width);

struct iarray_item_t*
iarray_find(
  const struct iarray_t* const array,
  const size_t x,
  const size_t y);

void
iarray_shift_x_right(
  struct iarray_t* const array,
  const size_t clicked_item_x,
  const size_t y,
  const size_t n);

void
iarray_shift_x_left(
  struct iarray_t* const array,
  const size_t clicked_item_x,
  const size_t y,
  const size_t n);

void
iarray_free(
  struct iarray_t** array);

#endif
