#ifndef INSTRUCTION_CREATE_RETURN_VALUE_H
#define INSTRUCTION_CREATE_RETURN_VALUE_H

// forward declaration
struct instruction_t;

struct instruction_create_return_value_t
{
  char* reference_function_name;
  char* value;
};

void
inst_create_return_value(
  struct instruction_t* dest_instruction,
  const char* const reference_function_name,
  const char* const return_value);

#endif
