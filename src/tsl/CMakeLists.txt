add_library(
  tsl 
  tsl.c
  # PARSER
  parser/parser.c
  parser/parser_internal.c
  parser/parser_actions.c
  parser/parser_instructions.c
  parser/parser_instructions_internal.c
  # INSTRUCTIONS
  instructions/execute_instructions.c
  instructions/instructions.c
  instructions/instructions_add_function_arg.c
  instructions/instructions_create_function.c
  instructions/instructions_create_var.c
  instructions/instructions_store_literal.c
  instructions/instructions_create_return_value.c
  instructions/instructions_store_var.c
  # MISC
  variable.c
  function.c
)
target_include_directories(
  tsl 
  PUBLIC 
  ${tml_SOURCE_DIR}/include/tsl
  ${tml_SOURCE_DIR}/include/tsl/parser
  ${tml_SOURCE_DIR}/include/tsl/instructions
)
