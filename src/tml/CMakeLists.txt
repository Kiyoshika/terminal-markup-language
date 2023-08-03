add_library(tml
  # AST
  ast/ast.c
  ast/ast_internal.c
  # PARSER
  parser/parser.c
  parser/parser_internal.c
  parser/parser_actions.c
  # MISC
  filesystem.c
  errors.c
  iarray.c
)
target_include_directories(
  tml 
  PUBLIC 
  ${tml_SOURCE_DIR}/include/tml
  ${tml_SOURCE_DIR}/include/tml/ast
  ${tml_SOURCE_DIR}/include/tml/parser)
target_link_libraries(tml ${CURSES_LIBRARY})

add_executable(tmlrender tml.c)
target_link_libraries(tmlrender tml tsl ${CURSES_LIBRARY})
