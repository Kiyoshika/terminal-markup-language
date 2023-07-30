#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

enum variable_type_e
{
  VAR_TYPE_INT,
  VAR_TYPE_FLOAT,
  VAR_TYPE_BOOL,
  VAR_TYPE_STRING,
  VAR_TYPE_NULL,
};

struct variable_t
{
  char* name;
  enum variable_type_e type;
  void* value;
};

struct variable_t*
var_create(
  const char* const name,
  const enum variable_type_e type,
  const void* const value,
  const size_t value_size);

#endif
