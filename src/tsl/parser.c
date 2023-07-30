#include "parser.h"

char tsl_tokens[TSL_N_TOKENS][TSL_MAX_TOKEN_LEN] = {
  // datatypes
  "int",
  "float",
  "bool",
  "string",

  // operators
  ">",
  ">=",
  "<",
  "<=",
  "=",
  "==",
  "!=",
  "!",
  "||",
  "&&",
  "+",
  "-",
  "*",
  "/",

  // brackets
  "(",
  ")",
  "{",
  "}",
  "[",
  "]",

  // misc
  ",",
  ";",
  "\"",
  " "
};

static bool
_parser_isspecial_token(
  const char current_char,
  const char next_char)
{
  switch (current_char)
  {
    case '=':
      return next_char == '=';

    case '!':
      return next_char == '=';

    case '>':
      return next_char == '=';

    case '<':
      return next_char == '=';

    case '&':
      return next_char == '&';

    case '|':
      return next_char == '|';
  }

  return false;
}

static bool
_parser_char_is_delimiter(
  const char current_char,
  const char next_char)
{
  if (isspace(current_char) || ispunct(current_char) || ispunct(next_char))
    return true;

  return false;
}

// false = early termination out of loop
static bool
_parser_append_char_to_current_token(
  struct parse_context_t* const context,
  const char current_char,
  const char next_char)
{
  if (isspace(current_char))
    return false;

  context->current_token_text[context->current_token_text_len++] = current_char;
  if (_parser_isspecial_token(current_char, next_char))
  {
    context->current_token_text[context->current_token_text_len++] = next_char;
    context->buffer_idx++;
    return false;
  }

  return true;
}

void
tsl_parser_parse(
  const char* const tsl_script)
{
  struct parse_context_t context = {
    .buffer = tsl_script,
    .buffer_idx = 0,
    .buffer_len = strlen(tsl_script),

    .current_state = TSL_STATE_NONE,
    .expected_state = TSL_STATE_CREATING_VAR | TSL_STATE_CREATING_FUNCTION,

    .current_token_text = {0},
    .current_token_text_len = 0,
    .current_token = TSL_TOKEN_NONE,
    .current_token_type = TSL_TOKEN_TYPE_NONE,

    .previous_token_text = {0},
    .previous_token = TSL_TOKEN_NONE,
    .previous_token_type = TSL_TOKEN_TYPE_NONE
  };

  while (context.buffer_idx < context.buffer_len)
  {
    tsl_parser_get_next_token(&context);
    printf("CURRENT TOKEN: %s (%d)\n", context.current_token_text, context.current_token);
  }
}

char
tsl_parser_peek_token(
  const struct parse_context_t* const context)
{
  if (context->buffer_idx == context->buffer_len)
    return 0;

  return context->buffer[context->buffer_idx];
}

void
tsl_parser_get_token_tags(
  struct parse_context_t* const context)
{
  context->previous_token = context->current_token;
  context->current_token = TSL_TOKEN_NONE;

  for (size_t i = 0; i < (size_t)TSL_N_TOKENS; ++i)
    if (strcmp(tsl_tokens[i], context->current_token_text) == 0)
      context->current_token = i;
}

void
tsl_parser_get_next_token(
  struct parse_context_t* const context)
{
  memset(context->previous_token_text, 0, TSL_MAX_TOKEN_LEN);
  strncat(context->previous_token_text, context->current_token_text, TSL_MAX_TOKEN_LEN);

  memset(context->current_token_text, 0, TSL_MAX_TOKEN_LEN);
  context->current_token_text_len = 0;

  // consume whitespace
  while (context->buffer[context->buffer_idx] == ' ')
    context->buffer_idx++;

  while (context->buffer_idx < context->buffer_len)
  {
    char current_char = context->buffer[context->buffer_idx++];
    char next_char = tsl_parser_peek_token(context);

    if (!_parser_append_char_to_current_token(context, current_char, next_char))
      break;

    if (_parser_char_is_delimiter(current_char, next_char))
    {
      if (_parser_isspecial_token(current_char, next_char))
        context->buffer_idx++;

      break;
    }
  }

  tsl_parser_get_token_tags(context);
}
