#ifndef TSL_H
#define TSL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "instructions.h"

// forward declarations
struct variable_list_t;
struct function_list_t;

struct tsl_global_scope_t
{
  struct variable_list_t* variable_list;
  struct function_list_t* function_list;
  struct instruction_list_t* instruction_list;
};

struct tsl_global_scope_t*
tsl_global_scope_create();

bool
tsl_global_add_instruction(
  struct tsl_global_scope_t* const global_scope,
  const enum instruction_type_e instruction_type,
  const struct instruction_t* const instruction);
  

void
tsl_global_execute_instructions();

#endif
