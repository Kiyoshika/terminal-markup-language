add_executable(tsl_var_true_value_bool tsl_var_true_value.c)
target_link_libraries(tsl_var_true_value_bool tsl)
add_test(NAME tsl_var_true_value_bool COMMAND tsl_var_true_value_bool)

add_executable(tsl_var_false_value_bool tsl_var_false_value.c)
target_link_libraries(tsl_var_false_value_bool tsl)
add_test(NAME tsl_var_false_value_bool COMMAND tsl_var_false_value_bool)

add_executable(tsl_var_uninitialized_value_bool tsl_var_uninitialized_value.c)
target_link_libraries(tsl_var_uninitialized_value_bool tsl)
add_test(NAME tsl_var_uninitialized_value_bool COMMAND tsl_var_uninitialized_value_bool)

add_executable(tsl_var_empty_name_bool tsl_var_empty_name.c)
target_link_libraries(tsl_var_empty_name_bool tsl)
add_test(NAME tsl_var_empty_name_bool COMMAND tsl_var_empty_name_bool)

add_executable(tsl_var_empty_value_bool tsl_var_empty_value.c)
target_link_libraries(tsl_var_empty_value_bool tsl)
add_test(NAME tsl_var_empty_value_bool COMMAND tsl_var_empty_value_bool)

add_executable(tsl_var_invalid_name_bool tsl_var_invalid_name.c)
target_link_libraries(tsl_var_invalid_name_bool tsl)
add_test(NAME tsl_var_invalid_name_bool COMMAND tsl_var_invalid_name_bool)

add_executable(tsl_var_valid_names_bool tsl_var_valid_names.c)
target_link_libraries(tsl_var_valid_names_bool tsl)
add_test(NAME tsl_var_valid_names_bool COMMAND tsl_var_valid_names_bool)

add_executable(tsl_var_already_exists_bool tsl_var_already_exists.c)
target_link_libraries(tsl_var_already_exists_bool tsl)
add_test(NAME tsl_var_already_exists_bool COMMAND tsl_var_already_exists_bool)

add_executable(tsl_var_assign_var_same_type_bool tsl_var_assign_var_same_type.c)
target_link_libraries(tsl_var_assign_var_same_type_bool tsl)
add_test(NAME tsl_var_assign_var_same_type_bool COMMAND tsl_var_assign_var_same_type_bool)

add_executable(tsl_var_assign_var_different_type_bool tsl_var_assign_var_different_type.c)
target_link_libraries(tsl_var_assign_var_different_type_bool tsl)
add_test(NAME tsl_var_assign_var_different_type_bool COMMAND tsl_var_assign_var_different_type_bool)

add_executable(tsl_var_assign_invalid_var_bool tsl_var_assign_invalid_var.c)
target_link_libraries(tsl_var_assign_invalid_var_bool tsl)
add_test(NAME tsl_var_assign_invalid_var_bool COMMAND tsl_var_assign_invalid_var_bool)
