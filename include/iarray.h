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

/* ncurses stores positions as ints which is why I'm not using size_t like I usually do */
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
  size_t x,
  size_t y);

void
iarray_free(
  struct iarray_t** array);

#endif
