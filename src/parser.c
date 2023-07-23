#include "parser.h"
#include "parser_internal.h"
#include "parser_actions.h"
#include "ast.h"
#include "filesystem.h"
#include "errors.h"

// IMPORTANT: this must be ordered the same as
// ast_node_type_e in ast.h
const char* tml_tag_names[N_TOKEN_TYPES] = {
  "tml",
  "text",
  "space",
  "input",
  "button"
};

// IMPORTANT: this must be ordered the same as
// ast_attribute_type_e in ast.h
const char* tml_attribute_names[N_ATTRIBUTE_TYPES] = {
  "fg",
  "bg",
  "newline",
  "bold",
  "callback",
  "minLength",
  "maxLength",
  "password",
  "marginLeft",
  "marginRight"
};

// IMPORTANT: this must be ordered the same as
// ast_attribute_value_e in ast.h
const char* tml_attribute_values[N_ATTRIBUTE_VALUES] = {
  "white",
  "black",
  "red",
  "blue",
  "yellow",
  "cyan",
  "green",
  "magenta",
  "true",
  "false",
  "int", // placeholder text, not actually used 
  "custom" // placeholder text, not actually used 
};

bool
parser_read_source_file(
  const char* const filepath,
  char* err_msg)
{
  char* source_buffer = fs_read_file(filepath);
  if (!source_buffer)
    return false;

  size_t tag_body_capacity = 100;

  struct parse_context_t context = {
    .root_node = NULL,
    .reference_node = NULL,
    .source_buffer = source_buffer,
    .source_buffer_len = strlen(source_buffer),
    .source_buffer_idx = 0,
    .state = TML_STATE_OPENING_TAG,
    .current_token = TML_TOKEN_NULL,
    .next_token = TML_TOKEN_NULL,
    .previous_token = TML_TOKEN_NULL,
    .expected_token = TML_TOKEN_OPEN_TAG | TML_TOKEN_SPACE,
    .tag_name = {0},
    .attribute_name = {0},
    .attribute_value = {0},
    .allowed_attribute_values = TML_ATTRIBUTE_VALUE_NONE,
    .tag_name_len = 0,
    .attribute_name_len = 0,
    .attribute_value_len = 0,
    .is_closing_tag = false,
    .tag_body_capacity = tag_body_capacity,
    .tag_body = calloc(tag_body_capacity, sizeof(char)),
    .tag_body_len = 0,
  };

  if (!context.tag_body)
    return false;

  bool success = parser_parse(&context, err_msg);
  free(source_buffer);

  return success;
}

enum ast_node_type_e
parser_get_node_type(
  const struct parse_context_t* const context)
{
  for (size_t i = 0; i < (size_t)N_TOKEN_TYPES; ++i)
    if (strcmp(tml_tag_names[i], context->tag_name) == 0)
      return (1 << i);

  return TML_NODE_NONE;
}

enum ast_attribute_value_e
parser_get_attribute_value(
  const struct parse_context_t* const context)
{
  for (size_t i = 0; i < (size_t)N_ATTRIBUTE_VALUES; ++i)
    if (strcmp(tml_attribute_values[i], context->attribute_value) == 0)
      return (1 << i);

  // WARNING: this does not check of over/underflow errors,
  // but realistically should not occur in "real" code.
  // (what're you gonna do, make a margin of 3 million pixels?)
  char* endptr = NULL;
  strtoul(context->attribute_value, &endptr, 10);
  if (strlen(endptr) == 0)
  {
    for (size_t i = 0; i < strlen(context->attribute_value); ++i)
      if (context->attribute_value[i] == '.')
        return TML_ATTRIBUTE_VALUE_NONE;

    return TML_ATTRIBUTE_VALUE_INT;
  }

  return TML_ATTRIBUTE_VALUE_CUSTOM;
}

enum ast_attribute_type_e
parser_get_attribute_type(
  const struct parse_context_t* const context)
{
  for (size_t i = 0; i < (size_t)N_ATTRIBUTE_TYPES; ++i)
    if (strcmp(tml_attribute_names[i], context->attribute_name) == 0)
      return (1 << i);

  return TML_ATTRIBUTE_NULL;
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

  if (isspace(current_char))
    return TML_TOKEN_SPACE;

  if ((current_char >= 'a' && current_char <= 'z')
      || (current_char >= 'A' && current_char <= 'Z'))
    return TML_TOKEN_TEXT;

  if (isdigit(current_char))
    return TML_TOKEN_NUMBER;

  if (ispunct(current_char))
    return TML_TOKEN_PUNCTUATION;

  return TML_TOKEN_NULL;
}

enum tml_token_type_e
parser_get_next_expected_token(
  const enum tml_token_type_e current_token)
{

  switch (current_token)
  {
    case TML_TOKEN_OPEN_TAG:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_SLASH;
    case TML_TOKEN_NUMBER:
    case TML_TOKEN_TEXT:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_EQUALS | TML_TOKEN_OPEN_TAG | TML_TOKEN_CLOSE_TAG | TML_TOKEN_NUMBER | TML_TOKEN_PUNCTUATION | TML_TOKEN_SLASH;
    case TML_TOKEN_CLOSE_TAG:
      return TML_TOKEN_SPACE | TML_TOKEN_OPEN_TAG | TML_TOKEN_TEXT | TML_TOKEN_NUMBER;
    case TML_TOKEN_EQUALS:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_NUMBER;
    case TML_TOKEN_SLASH:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_CLOSE_TAG;
    case TML_TOKEN_NULL:
      return TML_TOKEN_SPACE | TML_TOKEN_NULL;
    case TML_TOKEN_SPACE:
      return ~0u; // any token
    case TML_TOKEN_PUNCTUATION:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_EQUALS | TML_TOKEN_OPEN_TAG | TML_TOKEN_CLOSE_TAG | TML_TOKEN_NUMBER | TML_TOKEN_PUNCTUATION;
  }

  return TML_TOKEN_NULL;
}

bool
parser_perform_token_action(
  struct parse_context_t* context,
  char* err_msg)
{
  // making it static to persist throughout the parsing process
  static struct ast_t* current_node = NULL;
  const char current_char = context->source_buffer[context->source_buffer_idx];

  switch (context->current_token)
  {
    case TML_TOKEN_OPEN_TAG:
      return _parser_actions_open_tag(context, current_char, &current_node, err_msg);
      break;

    case TML_TOKEN_NUMBER:
    case TML_TOKEN_PUNCTUATION:
    case TML_TOKEN_TEXT:
      return _parser_actions_text(context, current_char, &current_node, err_msg);
      break;

    case TML_TOKEN_SPACE:
    {
      // ignore leading whitespace when parsing body (e.g., if you're heavily tabbed in)
      if (current_char == ' ' 
          && context->state == TML_STATE_PARSING_TAG_BODY
          && strlen(context->tag_body) > 0)
        return _parser_actions_text(context, current_char, &current_node, err_msg);

      return _parser_actions_space(context, current_char, &current_node, err_msg);
      break;
    }

    case TML_TOKEN_EQUALS:
      return _parser_actions_equals(context, current_char, &current_node, err_msg);
      break;

    case TML_TOKEN_CLOSE_TAG:
      return _parser_actions_close_tag(context, current_char, &current_node, err_msg);
      break;

    // no-op (shouldn't be encountered)
    case TML_TOKEN_NULL:
      break;

    // used for short-hand tokens, e.g., <space/>
    case TML_TOKEN_SLASH:
      return _parser_actions_slash(context, current_char, &current_node, err_msg);
      break;
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
  bool status = false;
  while (context->source_buffer_idx < context->source_buffer_len)
  {
    if (context->source_buffer_idx >= context->source_buffer_len)
      break;

    context->current_token 
      = parser_get_token_type(context->source_buffer[context->source_buffer_idx]);

    if (context->source_buffer_idx + 1 < context->source_buffer_len)
      context->next_token = parser_get_token_type(context->source_buffer[context->source_buffer_idx + 1]);
    else
      context->next_token = TML_TOKEN_NULL;

    if ((context->current_token & context->expected_token) == 0)
    {
      tml_error_unexpected_token(err_msg, context->source_buffer_idx);
      goto cleanup;
    }

    if (!parser_perform_token_action(context, err_msg))
      goto cleanup;

    parser_next_token(context);
  }

  status = true;
  ast_render(context->root_node);
cleanup:
  ast_free(&context->root_node);
  free(context->tag_body);

  return status;
}
