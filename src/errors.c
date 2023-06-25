#include "errors.h"

void
tml_error_unexpected_token(
  char* err_msg,
  const size_t token_position)
{
  sprintf(err_msg, "Unexpected token at position %zu.\n", token_position);
}

void
tml_error_unexpected_tag_name(
  char* err_msg,
  const char* const tag_name,
  const size_t token_position)
{
  sprintf(err_msg, "Unexpected tag name '%s' at position %zu.\n", tag_name, token_position);
}

void
tml_error_unexpected_attribute_name(
  char* err_msg,
  const char* const attribute_name,
  const size_t token_position)
{
  sprintf(err_msg, "Unexpected attribute name '%s' at position %zu.\n", attribute_name, token_position);
}

void
tml_error_empty_attribute_value(
  char* err_msg,
  const char* const attribute_name,
  const size_t token_position)
{
  sprintf(err_msg, "Empty attribte value for attribute '%s' at position %zu.\n", attribute_name, token_position);
}

void
tml_error_node_failure(
  char* err_msg)
{
  sprintf(err_msg, "There was an internal error when creating nodes (e.g., out of memory).\n");
}

void
tml_error_missing_root_node(
  char* err_msg)
{
  sprintf(err_msg, "Missing root node <tml>.\n");
}

void
tml_error_empty_tag_name(
  char* err_msg,
  const size_t token_position)
{
  sprintf(err_msg, "Empty tag name at position %zu.\n", token_position);
}

void
tml_error_root_already_exists(
  char* err_msg)
{
  sprintf(err_msg, "Root node <tml> already exists.\n");
}

void
tml_error_close_tag_not_matching_parent(
  char* err_msg,
  const char* const tag_name,
  const size_t token_position)
{
  sprintf(err_msg, "Closing tag </%s> at position %zu does not match its parent.\n", tag_name, token_position);
}

void
tml_error_disallowed_child_type(
  char* err_msg,
  const char* const tag_name,
  const size_t token_position)
{
  sprintf(err_msg, "Tag <%s> at position %zu cannot be nested with its current parent tag.\n", tag_name, token_position);
}
