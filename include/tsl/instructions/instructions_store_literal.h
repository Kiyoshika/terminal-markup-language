#ifndef INSTRUCTIONS_STORE_LITERAL_H
#define INSTRUCTIONS_STORE_LITERAL_H

#include "variable.h"

// forward declaraction
struct function_t;
struct instruction_t;

// e.g., myVar = 12; (variable presumably already exists)
struct instruction_store_literal_t
{
  struct function_t* reference_function;

  char* variable_name;
  enum variable_type_e variable_type;
  union variable_value
  {
    int32_t as_int;
    float as_float;
    bool as_bool;
    char* as_string;
  } variable_value;
};

void
inst_store_literal_int(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const int32_t variable_value);

void
inst_store_literal_float(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const float variable_value);

void
inst_store_literal_bool(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const bool variable_value);

void
inst_store_literal_string(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const char* const variable_value);

#endif
