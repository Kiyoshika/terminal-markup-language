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
_parser_reset_body(
  struct parse_context_t* context)
{
  memset(context->tag_body, 0, context->tag_body_capacity);
  context->tag_body_len = 0;
}

void
_parser_reset_tag_state(
  struct parse_context_t* context)
{
  _parser_reset_tag_name(context);
  _parser_reset_attribute(context);
  _parser_reset_body(context);
}

bool
_parser_append_tag_body_char(
  struct parse_context_t* context,
  const char c)
{
  context->tag_body[context->tag_body_len++] = c;
  if (context->tag_body_len == context->tag_body_capacity)
  {
    size_t new_capacity = context->tag_body_capacity * 2;
    void* alloc = realloc(context->tag_body, new_capacity * sizeof(char));
    if (!alloc)
      return false;
    context->tag_body = alloc;
    // realloc doesn't guarantee clean zeros
    for (size_t i = context->tag_body_len; i < context->tag_body_capacity; ++i)
      context->tag_body[i] = '\0';
  }

  return true;
}
