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
  union value
  {
    int32_t as_int;
    float as_float;
    bool as_bool;
    char* as_string;
  } value;
};

struct variable_list_t
{
  struct variable_t* variables;
  size_t n_variables;
  size_t capacity;
};

struct variable_t*
var_create_int(
  const char* const name,
  const int32_t value);

struct variable_t*
var_create_float(
  const char* const name,
  const float value);

struct variable_t*
var_create_bool(
  const char* const name,
  const bool value);

struct variable_t*
var_create_string(
  const char* const name,
  const char* const value);

struct variable_list_t*
var_list_create();

bool
var_list_add_variable(
  struct variable_list_t* const variable_list,
  const struct variable_t* const variable);

struct variable_t*
var_list_find(
  const struct variable_list_t* const variable_list,
  const char* const variable_name);

#endif
