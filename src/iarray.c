#include "iarray.h"
#include "ast.h"

struct iarray_t*
iarray_create()
{
  size_t capacity = 10;
  struct iarray_t* array = malloc(sizeof(*array));
  if (!array)
    return NULL;

  array->items = calloc(capacity, sizeof(*array->items));
  if (!array->items)
  {
    free(array);
    return NULL;
  }

  array->length = 0;
  array->capacity = capacity;

  return array;
}

bool
iarray_add(
  struct iarray_t* const array,
  struct ast_t* node,
  size_t x,
  size_t y,
  size_t width)
{
  // avoid adding duplicate items
  for (size_t i = 0; i < array->length; ++i)
    if (&*array->items[i].node == &*node)
      return true;

  struct iarray_item_t new_item = {
    .node = node,
    .x = x,
    .y = y,
    .width = width
  };

  memcpy(&array->items[array->length++], &new_item, sizeof(new_item));
  if (array->length == array->capacity)
  {
    size_t new_capacity = array->capacity * 2;
    void* alloc = realloc(array->items, new_capacity * sizeof(new_item));
    if (!alloc)
      return false;
    array->items = alloc;
    array->capacity = new_capacity;
  }

  return true;
}

struct iarray_item_t*
iarray_find(
  const struct iarray_t* const array,
  size_t x,
  size_t y)
{
  for (size_t i = 0; i < array->length; ++i)
  {
      if (array->items[i].y == y
          && (x >= array->items[i].x && x < array->items[i].x + array->items[i].width))
        return &array->items[i];
  }

  return NULL;
}

void
iarray_free(
  struct iarray_t** array)
{
  free((*array)->items);
  (*array)->items = NULL;

  free(*array);
  *array = NULL;
}
