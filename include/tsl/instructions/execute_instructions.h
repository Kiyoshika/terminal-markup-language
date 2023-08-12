#ifndef EXECUTE_INSTRUCTIONS_H
#define EXECUTE_INSTRUCTIONS_H

#include <stdbool.h>
#include <stddef.h>

// forward declaration
struct tsl_global_scope_t;

bool
tsl_execute_instructions_create_var(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx);

bool
tsl_execute_instructions_store_literal(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx);

bool
tsl_execute_instructions_store_var(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx);

bool
tsl_execute_instructions_create_function(
  struct tsl_global_scope_t* const global_scope,
  const size_t instruction_idx);

#endif
