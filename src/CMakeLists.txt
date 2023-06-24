add_library(tml
  filesystem.c
  colours.c
  ast.c
  parser.c
  parser_internal.c
  errors.c
)
target_include_directories(tml PUBLIC ${tml_SOURCE_DIR}/include)

add_executable(tmlrender tml.c)
target_link_libraries(tmlrender tml ${CURSES_LIBRARY})
