add_library(tml
  filesystem.c
  ast.c
  ast_internal.c
  parser.c
  parser_internal.c
  parser_actions.c
  errors.c
  iarray.c
)
target_include_directories(tml PUBLIC ${tml_SOURCE_DIR}/include)
target_link_libraries(tml ${CURSES_LIBRARY})

add_executable(tmlrender tml.c)
target_link_libraries(tmlrender tml ${CURSES_LIBRARY})
