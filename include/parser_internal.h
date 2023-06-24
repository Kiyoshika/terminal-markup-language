#ifndef TML_PARSER_INTERNAL_H
#define TML_PARSER_INTERNAL_H

struct parse_context_t;

/*
 * Side note about append functions:
 * each buffer is memset zero'd each reset,
 * so by default they're null terminated.
 * This is why no manual \0 appending is done.
 */

void
_parser_append_tag_name_char(
  struct parse_context_t* context,
  const char c);

void
_parser_append_attribute_name_char(
  struct parse_context_t* context,
  const char c);

void
_parser_append_attribute_value_char(
  struct parse_context_t* context,
  const char c);

void
_parser_reset_tag_name(
  struct parse_context_t* context);

void
_parser_reset_attribute(
  struct parse_context_t* context);

void
_parser_reset_tag_state(
  struct parse_context_t* context);

#endif
