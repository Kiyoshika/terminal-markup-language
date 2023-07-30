#include "parser.h"

char tsl_datatypes[TSL_N_DATATYPES][TSL_MAX_TOKEN_LEN] = {
  "int",
  "float",
  "string",
  "bool"
};

char tsl_operators[TSL_N_OPERATORS][TSL_MAX_TOKEN_LEN] = {
  "=",
  "==",
  "!=",
  ">",
  ">=",
  "<",
  "<=",
  "&",
  "!",
  "&&",
  "||"
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
  if (isspace(current_char) || ispunct(current_char) || next_char == ';')
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
  context->current_token[context->current_token_len++] = current_char;
  if (_parser_isspecial_token(current_char, next_char))
  {
    context->current_token[context->current_token_len++] = next_char;
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

    .current_token = {0},
    .current_token_len = 0,

    .previous_token = {0}
  };

  while (context.buffer_idx < context.buffer_len)
  {
    tsl_parser_get_next_token(&context);
    printf("CURRENT TOKEN: %s\n", context.current_token);
    printf("PREVIOUS TOKEN: %s\n\n", context.previous_token);
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
tsl_parser_get_next_token(
  struct parse_context_t* const context)
{

  memset(context->previous_token, 0, TSL_MAX_TOKEN_LEN);
  strncat(context->previous_token, context->current_token, TSL_MAX_TOKEN_LEN);

  memset(context->current_token, 0, TSL_MAX_TOKEN_LEN);
  context->current_token_len = 0;

  while (context->buffer_idx < context->buffer_len)
  {
    char current_char = context->buffer[context->buffer_idx++];
    char next_char = tsl_parser_peek_token(context);

    if (_parser_char_is_delimiter(current_char, next_char))
    {
      _parser_append_char_to_current_token(context, current_char, next_char);
      break;
    }

    if (!_parser_append_char_to_current_token(context, current_char, next_char))
      break;
    
  }
}
