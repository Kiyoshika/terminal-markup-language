#include "parser.h"
#include "parser_internal.h"

void
_parser_append_tag_name_char(
  struct parse_context_t* context,
  const char c)
{
  if (context->tag_name_len >= (size_t)TML_TAG_NAME_LEN - 1)
    return;

  context->tag_name[context->tag_name_len++] = c;
}

void
_parser_append_attribute_name_char(
  struct parse_context_t* context,
  const char c)
{
  if (context->attribute_name_len >= (size_t)TML_ATTRIBUTE_NAME_LEN - 1)
    return;

  context->attribute_name[context->attribute_name_len++] = c;
}

void
_parser_append_attribute_value_char(
  struct parse_context_t* context,
  const char c)
{
  if (context->attribute_value_len >= (size_t)TML_ATTRIBUTE_VALUE_LEN - 1)
    return;

  context->attribute_value[context->attribute_value_len++] = c;
}

void
_parser_reset_tag_name(
  struct parse_context_t* context)
{
  memset(context->tag_name, 0, TML_TAG_NAME_LEN);
  context->tag_name_len = 0;
}

void
_parser_reset_attribute(
  struct parse_context_t* context)
{
  memset(context->attribute_name, 0, TML_ATTRIBUTE_NAME_LEN);
  context->attribute_name_len = 0;

  memset(context->attribute_value, 0, TML_ATTRIBUTE_VALUE_LEN);
  context->attribute_value_len = 0;
}

void
_parser_reset_tag_state(
  struct parse_context_t* context)
{
  _parser_reset_tag_name(context);
  _parser_reset_attribute(context);
}
