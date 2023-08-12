#ifndef INSTRUCTION_STORE_VAR_H
#define INSTRUCTION_STORE_VAR_H

// e.g., myVar = otherVar;
struct instruction_store_var_t
{
  struct function_t* reference_function;

  char* dest_variable_name;
  char* src_variable_name;
};

void
inst_store_var(
  struct instruction_t* dest_instruction,
  struct function_t** reference_function,
  const char* const dest_variable_name,
  const char* const src_variable_name);

#endif
