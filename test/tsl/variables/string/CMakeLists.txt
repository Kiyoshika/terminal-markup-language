add_executable(tsl_var_string_noquotes tsl_var_string_noquotes.c)
target_link_libraries(tsl_var_string_noquotes tsl)
add_test(NAME tsl_var_string_noquotes COMMAND tsl_var_string_noquotes)

add_executable(tsl_var_string_valid tsl_var_string_valid.c)
target_link_libraries(tsl_var_string_valid tsl)
add_test(NAME tsl_var_string_valid COMMAND tsl_var_string_valid)

add_executable(tsl_var_already_exists_string tsl_var_already_exists.c)
target_link_libraries(tsl_var_already_exists_string tsl)
add_test(NAME tsl_var_already_exists_string COMMAND tsl_var_already_exists_string)

add_executable(tsl_var_assign_invalid_var_string tsl_var_assign_invalid_var.c)
target_link_libraries(tsl_var_assign_invalid_var_string tsl)
add_test(NAME tsl_var_assign_invalid_var_string COMMAND tsl_var_assign_invalid_var_string)

add_executable(tsl_var_assign_var_different_type_string tsl_var_assign_var_different_type.c)
target_link_libraries(tsl_var_assign_var_different_type_string tsl)
add_test(NAME tsl_var_assign_var_different_type_string COMMAND tsl_var_assign_var_different_type_string)

add_executable(tsl_var_assign_var_same_type_string tsl_var_assign_var_same_type.c)
target_link_libraries(tsl_var_assign_var_same_type_string tsl)
add_test(NAME tsl_var_assign_var_same_type_string COMMAND tsl_var_assign_var_same_type_string)

add_executable(tsl_var_empty_name_string tsl_var_empty_name.c)
target_link_libraries(tsl_var_empty_name_string tsl)
add_test(NAME tsl_var_empty_name_string COMMAND tsl_var_empty_name_string)

add_executable(tsl_var_empty_value_string tsl_var_empty_value.c)
target_link_libraries(tsl_var_empty_value_string tsl)
add_test(NAME tsl_var_empty_value_string COMMAND tsl_var_empty_value_string)

add_executable(tsl_var_uninitialized_value_string tsl_var_uninitialized_value.c)
target_link_libraries(tsl_var_uninitialized_value_string tsl)
add_test(NAME tsl_var_uninitialized_value_string COMMAND tsl_var_uninitialized_value_string)

add_executable(tsl_var_valid_names_string tsl_var_valid_names.c)
target_link_libraries(tsl_var_valid_names_string tsl)
add_test(NAME tsl_var_valid_names_string COMMAND tsl_var_valid_names_string)
