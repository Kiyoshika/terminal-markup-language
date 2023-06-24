#include "parser.h"
#include "ast.h"
#include "filesystem.h"

// IMPORTANT: this must be ordered the same as
// ast_node_type_e in ast.h
const char* tml_token_text[N_TOKEN_TYPES] = {
  "text",
  "tml"
};

// IMPORTANT: this must be ordered the same as
// tml_attribute_type_e in parser.h
const char* tml_attribute_text[N_ATTRIBUTE_TYPES] = {
  "fg",
  "bg"
};

bool
parser_read_source_file(
  const char* const filepath,
  char* err_msg)
{
  char* source_buffer = fs_read_file(filepath);
  if (!source_buffer)
    return false;

  struct parse_context_t context = {
    .source_buffer = source_buffer,
    .source_buffer_len = strlen(source_buffer),
    .source_buffer_idx = 0,
    .state = TML_STATE_OPENING_TAG,
    .current_token = TML_TOKEN_NULL,
    .previous_token = TML_TOKEN_NULL,
    .expected_token = TML_TOKEN_OPEN_TAG
  };

  bool success = parser_parse(&context, err_msg);
  free(source_buffer);

  return success;
}

enum ast_node_type_e
parser_get_node_type(
  const char* const tag_text)
{
  for (size_t i = 0; i < (size_t)N_TOKEN_TYPES; ++i)
    if (strcmp(tml_token_text[i], tag_text) == 0)
      return i;

  return -1;
}

enum tml_attribute_type_e
parser_get_attribute_type(
  const char* const attribute_text)
{
  for (size_t i = 0; i < (size_t)N_ATTRIBUTE_TYPES; ++i)
    if (strcmp(tml_attribute_text[i], attribute_text) == 0)
      return (1 << i);

  return -1;
}

enum tml_token_type_e
parser_get_token_type(
  const char current_char)
{
  switch (current_char)
  {
    case '<':
      return TML_TOKEN_OPEN_TAG;
    case '>':
      return TML_TOKEN_CLOSE_TAG;
    case '=':
      return TML_TOKEN_EQUALS;
    case '/':
      return TML_TOKEN_SLASH;
  }

  if ((current_char >= 'a' && current_char <= 'z')
      || (current_char >= 'A' && current_char <= 'Z'))
    return TML_TOKEN_TEXT;

  return TML_TOKEN_NULL;
}

void
parser_consume_whitespace(
  struct parse_context_t* context)
{
  size_t original_idx = context->source_buffer_idx;
  while (isspace(context->source_buffer[context->source_buffer_idx++]));
  if (context->source_buffer_idx > original_idx)
    context->source_buffer_idx--;
}

enum tml_token_type_e
parser_get_next_expected_token(
  const enum tml_token_type_e current_token)
{
  switch (current_token)
  {
    case TML_TOKEN_OPEN_TAG:
      return TML_TOKEN_TEXT | TML_TOKEN_SLASH;
    case TML_TOKEN_TEXT:
      return TML_TOKEN_TEXT | TML_TOKEN_CLOSE_TAG;
    case TML_TOKEN_CLOSE_TAG:
      return TML_TOKEN_OPEN_TAG;
    case TML_TOKEN_EQUALS:
      return TML_TOKEN_TEXT;
    case TML_TOKEN_SLASH:
      return TML_TOKEN_TEXT;
    case TML_TOKEN_NULL:
      return TML_TOKEN_NULL;
  }

  return TML_TOKEN_NULL;
}

bool
parser_perform_token_action(
  struct parse_context_t* context,
  char* err_msg)
{
  switch (context->current_token)
  {
    case TML_TOKEN_OPEN_TAG:
    {
      if ((context->state & TML_STATE_OPENING_TAG) == 0)
      {
        sprintf(err_msg, "Expected opening tag '<'.");
        return false;
      }

      context->state = TML_STATE_PARSING_TAG_NAME;
      return true;
    }
  }

  return true;
}

void
parser_next_token(
  struct parse_context_t* context)
{
  context->previous_token = context->current_token;
  context->expected_token = parser_get_next_expected_token(context->current_token);
  context->source_buffer_idx++;
}

bool
parser_parse(
  struct parse_context_t* context,
  char* err_msg)
{
  while (context->source_buffer_idx < context->source_buffer_len)
  {
    parser_consume_whitespace(context);
    if (context->source_buffer_idx >= context->source_buffer_len)
      break;

    context->current_token 
      = parser_get_token_type(context->source_buffer[context->source_buffer_idx]);

    if ((context->current_token & context->expected_token) == 0)
    {
      sprintf(err_msg, "Unexpected token at pos %zu.", context->source_buffer_idx);
      return false;
    }

    if (!parser_perform_token_action(context, err_msg))
      return false;

    parser_next_token(context);
  }

  return true;
}
