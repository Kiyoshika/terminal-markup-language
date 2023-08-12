#include "instructions.h"
#include "function.h"

struct instruction_list_t*
inst_list_create()
{
  struct instruction_list_t* instruction_list
    = malloc(sizeof(*instruction_list));

  if (!instruction_list)
    return NULL;

  instruction_list->n_instructions = 0;
  instruction_list->capacity = 10;
  instruction_list->instructions 
    = calloc(
        instruction_list->capacity, 
        sizeof(*instruction_list->instructions));
  if (!instruction_list->instructions)
  {
    free(instruction_list);
    return NULL;
  }

  return instruction_list;
}

bool
inst_list_add_instruction(
  struct instruction_list_t* instruction_list,
  const struct instruction_t* const instruction)
{
  memcpy(
    &instruction_list->instructions[instruction_list->n_instructions++],
    instruction,
    sizeof(*instruction));
  
  if (instruction_list->n_instructions == instruction_list->capacity)
  {
    size_t new_capacity = instruction_list->capacity * 2;
    void* alloc = realloc(instruction_list->instructions, new_capacity * sizeof(*instruction_list->instructions));
    if (!alloc)
      return false;
    instruction_list->instructions = alloc;
    instruction_list->capacity = new_capacity;
  }

  return true;
}
