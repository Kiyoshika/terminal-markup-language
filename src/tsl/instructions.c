#include "instructions.h"

struct instruction_list_t*
inst_create_list()
{
  struct instruction_list_t* instruction_list
    = malloc(sizeof(*instruction_list));

  if (!instruction_list)
    return NULL;

  instruction_list->n_instructions = 0;
  instruction_list->capacity = 10;
  instruction_list->instructions 
    = calloc(
        instruction_list->n_instructions, 
        sizeof(*instruction_list->instructions));
  if (!instruction_list->instructions)
  {
    free(instruction_list);
    return NULL;
  }
}

void
inst_add_instruction(
  struct instruction_list_t* instruction_list,
  const enum instruction_type_e instruction_type,
  void* instruction_struct)
{
  struct instruction_t new_instruction;
  new_instruction.instruction_type = instruction_type;

  switch (instruction_type)
  {
    case INST_TYPE_CREATE_VAR:
    {
      struct instruction_create_var_t* create_var = instruction_struct;
      memcpy(&new_instruction.instruction.create_var, create_var, sizeof(*create_var));
      break;
    }

    case INST_TYPE_STORE_LITERAL:
    {
      struct instruction_store_literal_t* store_literal = instruction_struct;
      memcpy(&new_instruction.instruction.store_literal, instruction_struct, sizeof(*store_literal));
      break;
    }

    case INST_TYPE_STORE_VAR:
    {
      struct instruction_store_var_t* store_var = instruction_struct;
      memcpy(&new_instruction.instruction.store_var, instruction_struct, sizeof(*store_var));
      break;
    }
  }

  memcpy(
    &instruction_list->instructions[instruction_list->n_instructions++],
    &new_instruction,
    sizeof(new_instruction));
  
  if (instruction_list->n_instructions == instruction_list->capacity)
  {
    size_t new_capacity = instruction_list->capacity * 2;
    void* alloc = realloc(instruction_list->instructions, new_capacity * sizeof(*instruction_list->instructions));
    if (!alloc)
      return;
    instruction_list->instructions = alloc;
    instruction_list->capacity = new_capacity;
  }
}
