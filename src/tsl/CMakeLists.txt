add_library(
  tsl 
  tsl.c
  # PARSER
  parser/parser.c
  parser/parser_internal.c
  parser/parser_actions.c
  parser/parser_instructions.c
  parser/parser_instructions_internal.c
  # MISC
  variable.c
  instructions.c
  function.c
)
target_include_directories(
  tsl 
  PUBLIC 
  ${tml_SOURCE_DIR}/include/tsl
  ${tml_SOURCE_DIR}/include/tsl/parser) 
