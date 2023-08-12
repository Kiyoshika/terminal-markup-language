#ifndef FUNCTION_H
#define FUNCTION_H

#define FUNC_NAME_MAX_LEN 51

#include "variable.h"
#include "instructions.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

// forward declaration
struct variable_list_t;
struct instruction_list_t;
struct instruction_t;

struct function_t
{
  enum variable_type_e return_type;

  char name[FUNC_NAME_MAX_LEN];

  // NOTE: function parameters are inserted here first
  struct variable_list_t* variable_list;

  struct instruction_list_t* instruction_list;
};

struct function_list_t
{
  struct function_t* functions;
  size_t n_functions;
  size_t capacity;
};

struct function_t*
func_create(
  const enum variable_type_e return_type,
  const char* const name);

bool
func_add_variable(
  struct function_t* const function,
  const struct variable_t* const variable);

bool
func_add_instruction(
  struct function_t* const function,
  const struct instruction_t* const instruction);

struct function_list_t*
func_list_create();

bool
func_list_add_function(
  struct function_list_t* const function_list,
  const struct function_t* const function);

struct function_t*
func_list_find(
  const struct function_list_t* const function_list,
  const char* const function_name);

#endif
