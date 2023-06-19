add_executable(ast_create ast_create.c)
target_include_directories(ast_create PUBLIC ${tml_SOURCE_DIR}/include)
target_link_libraries(ast_create tml)
add_test(NAME ast_create COMMAND ast_create)
