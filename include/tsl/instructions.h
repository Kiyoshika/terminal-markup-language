#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "variable.h"

// forward declaration
struct function_t;

enum instruction_type_e
{
  INST_TYPE_CREATE_VAR, // create new variable
  INST_TYPE_STORE_LITERAL, // store literal into variable
  INST_TYPE_STORE_VAR // store value from variable into variable
};

// e.g., int myVar;
// when initializing, e.g., int x = 5; this is the first instruction performed followed by a store literal/variable instruction
struct instruction_create_var_t
{
  struct function_t* reference_function;

  char* variable_name;
  enum variable_type_e variable_type;
};

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

// e.g., myVar = otherVar; (both variables presumably already exist)
struct instruction_store_var_t
{
  struct function_t* reference_function;

  char* dest_variable_name;
  char* src_variable_name;
};

struct instruction_t
{
  enum instruction_type_e instruction_type;
  union instruction
  {
    struct instruction_create_var_t create_var;
    struct instruction_store_literal_t store_literal;
    struct instruction_store_var_t store_var;
  } instruction;
};

struct instruction_list_t 
{
  struct instruction_t* instructions;
  size_t n_instructions;
  size_t capacity;
};

void
inst_create_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const variable_name,
  const enum variable_type_e variable_type);

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

void
inst_store_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const dest_variable_name,
  const char* const src_variable_name);

struct instruction_list_t*
inst_list_create();

bool
inst_list_add_instruction(
  struct instruction_list_t* instruction_list,
  const struct instruction_t* const instruction);

#endif
