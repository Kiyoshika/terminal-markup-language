#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "variable.h"

// other instructions
#include "instructions_create_var.h"
#include "instructions_store_literal.h"
#include "instructions_store_var.h"
#include "instructions_create_function.h"
#include "instructions_add_function_arg.h"

// forward declaration
struct function_t;

enum instruction_type_e
{
  INST_TYPE_CREATE_VAR, // create new variable
  INST_TYPE_STORE_LITERAL, // store literal into variable
  INST_TYPE_STORE_VAR, // store value from variable into variable
  INST_TYPE_CREATE_FUNCTION, // create new function
  INST_TYPE_ADD_FUNCTION_ARG, // add argument to function
};

struct instruction_t
{
  enum instruction_type_e instruction_type;
  union instruction
  {
    struct instruction_create_var_t       create_var;
    struct instruction_store_literal_t    store_literal;
    struct instruction_store_var_t        store_var;
    struct instruction_create_function_t  create_function;
    struct instruction_add_function_arg_t add_function_arg;
  } instruction;
};

struct instruction_list_t 
{
  struct instruction_t* instructions;
  size_t n_instructions;
  size_t capacity;
};

struct instruction_list_t*
inst_list_create();

bool
inst_list_add_instruction(
  struct instruction_list_t* instruction_list,
  const struct instruction_t* const instruction);

#endif
