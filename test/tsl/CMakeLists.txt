add_executable(tslsandbox tslsandbox.c)
target_link_libraries(tslsandbox tsl)
target_include_directories(tslsandbox PUBLIC ${tml_SOURCE_DIR}/include/tsl)
