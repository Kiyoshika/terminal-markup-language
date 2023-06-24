#ifndef TML_ERRORS_H
#define TML_ERRORS_H

#include <stddef.h>
#include <stdio.h>

void
tml_error_unexpected_token(
  char* err_msg,
  const size_t token_position);

void
tml_error_unexpected_tag_name(
  char* err_msg,
  const char* const tag_name,
  const size_t token_position);

void
tml_error_unexpected_attribute_name(
  char* err_msg,
  const char* const tag_name,
  const size_t token_position);

void
tml_error_empty_attribute_value(
  char* err_msg,
  const char* const attribute_name,
  const size_t token_position);

void
tml_error_node_failure(
  char* err_msg);

void
tml_error_missing_root_node(
  char* err_msg);

void
tml_error_empty_tag_name(
  char* err_msg,
  const size_t token_position);

void
tml_error_root_already_exists(
  char* err_msg);

#endif
