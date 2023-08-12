#include "parser.h"
#include "tsl.h"
#include "instructions.h"
#include "parser_actions.h"
#include "parser_internal.h"

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

static enum token_type_e
_parser_get_token_type(
  const enum token_e current_token)
{
  switch (current_token)
  {
    case TSL_TOKEN_INT:
    case TSL_TOKEN_FLOAT:
    case TSL_TOKEN_BOOL:
    case TSL_TOKEN_STRING:
      return TSL_TOKEN_TYPE_DATATYPE;

    case TSL_TOKEN_GREATER_THAN:
    case TSL_TOKEN_GREATER_THAN_EQUAL:
    case TSL_TOKEN_LESS_THAN:
    case TSL_TOKEN_LESS_THAN_EQUAL:
    case TSL_TOKEN_ASSIGN: // =
    case TSL_TOKEN_EQUAL: // ==
    case TSL_TOKEN_NOT_EQUAL:
    case TSL_TOKEN_LOGICAL_NOT:
    case TSL_TOKEN_LOGICAL_OR:
    case TSL_TOKEN_LOGICAL_AND:
    case TSL_TOKEN_PLUS:
    case TSL_TOKEN_MINUS:
    case TSL_TOKEN_MULTIPLY:
    case TSL_TOKEN_DIVIDE:
      return TSL_TOKEN_TYPE_OPERATOR;

    case TSL_TOKEN_OPEN_PAREN:
    case TSL_TOKEN_CLOSE_PAREN:
    case TSL_TOKEN_OPEN_BODY: // {
    case TSL_TOKEN_CLOSE_BODY: // }
    case TSL_TOKEN_OPEN_ARRAY: // [
    case TSL_TOKEN_CLOSE_ARRAY: // ]
      return TSL_TOKEN_TYPE_BRACKET;

    case TSL_TOKEN_COMMA:
    case TSL_TOKEN_SEMICOLON:
    case TSL_TOKEN_QUOTE:
    case TSL_TOKEN_SPACE:
      return TSL_TOKEN_TYPE_MISC;
  }

  return TSL_TOKEN_TYPE_TEXT;
}

struct tsl_global_scope_t*
tsl_parser_parse(
  const char* const tsl_script)
{
  // TODO: add expected token checks
  // (e.g., prevent int x 10 instead of int x = 10 by checking expected tokens)

  struct parse_context_t context = {
    .global_scope = tsl_global_scope_create(),

    .buffer = tsl_script,
    .buffer_idx = 0,
    .buffer_len = strlen(tsl_script),

    .current_state = TSL_STATE_NONE,
    .expected_state = TSL_STATE_NONE,

    .current_token_text = {0},
    .current_token_text_len = 0,
    .current_token = TSL_TOKEN_NONE,
    .current_token_type = TSL_TOKEN_TYPE_NONE,

    .previous_token_text = {0},
    .previous_token = TSL_TOKEN_NONE,
    .previous_token_type = TSL_TOKEN_TYPE_NONE,

    .datatype = TSL_TOKEN_NONE,
    .object_name = {0},
    .assigning_value = false,
    .object_value = {0},

    .inside_quotes = false,
    .is_string_literal = false
  };

  while (context.buffer_idx < context.buffer_len)
  {
    tsl_parser_get_next_token(&context);
    printf("CURRENT TOKEN: %s (%d)\n", context.current_token_text, context.current_token);
    if (!tsl_parser_perform_action(&context))
    {
      tsl_global_scope_free(&context.global_scope);
      return NULL;
    }
  }

  if (!tsl_global_scope_execute_instructions(context.global_scope))
  {
    tsl_global_scope_free(&context.global_scope);
    return NULL;
  }

  return context.global_scope;
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
  context->previous_token_type = context->current_token_type;

  context->current_token = TSL_TOKEN_NONE;

  for (size_t i = 0; i < (size_t)TSL_N_TOKENS; ++i)
    if (strcmp(tsl_tokens[i], context->current_token_text) == 0)
      context->current_token = i;

  context->current_token_type = _parser_get_token_type(context->current_token);
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
  {
    if (context->assigning_value && context->inside_quotes)
      strncat(context->object_value, " ", TSL_MAX_TOKEN_LEN);
    context->buffer_idx++;
  }

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

bool
tsl_parser_perform_action(
  struct parse_context_t* const context)
{

  switch (context->current_token_type)
  {
    case TSL_TOKEN_TYPE_DATATYPE:
      return tsl_parser_actions_datatype(context);
      break;

    case TSL_TOKEN_TYPE_TEXT:
      return tsl_parser_actions_text(context);
      break;
  }

  switch (context->current_token)
  {
    case TSL_TOKEN_ASSIGN:
      return tsl_parser_actions_assign(context);
      break;

    case TSL_TOKEN_MINUS:
      return tsl_parser_actions_minus(context); 
      break;

    case TSL_TOKEN_SEMICOLON:
      return tsl_parser_actions_semicolon(context);
      break;

    case TSL_TOKEN_QUOTE:
      return tsl_parser_actions_quote(context);
      break;

    case TSL_TOKEN_COMMA:
      return tsl_parser_actions_comma(context);
      break;

    case TSL_TOKEN_OPEN_PAREN:
      return tsl_parser_actions_open_paren(context);
      break;

    case TSL_TOKEN_CLOSE_PAREN:
      return tsl_parser_actions_close_paren(context);
      break;

    case TSL_TOKEN_OPEN_BODY:
      return tsl_parser_actions_open_body(context);
      break;

    case TSL_TOKEN_CLOSE_BODY:
      return tsl_parser_actions_close_body(context);
      break;

    // any other unrecognized token will be added a string literal (if parsing)
    default:
    {
      if (context->assigning_value && context->inside_quotes)
      {
        strncat(context->object_value, context->current_token_text, TSL_MAX_TOKEN_LEN);
        return true;
      }
      break;
    }
  }
  return false;
}

void
tsl_parser_reset_state(
  struct parse_context_t* const context)
{
  context->current_state = TSL_STATE_NONE;
  context->expected_state = TSL_STATE_NONE;

  memset(context->current_token_text, 0, TSL_MAX_TOKEN_LEN);
  context->current_token_text_len = 0;
  context->current_token = TSL_TOKEN_NONE;
  context->current_token_type = TSL_TOKEN_TYPE_NONE;

  memset(context->previous_token_text, 0, TSL_MAX_TOKEN_LEN);
  context->previous_token = TSL_TOKEN_NONE;
  context->previous_token_type = TSL_TOKEN_TYPE_NONE;

  context->datatype = TSL_TOKEN_NONE;
  memset(context->object_name, 0, TSL_MAX_TOKEN_LEN);
  context->assigning_value = false;
  memset(context->object_value, 0, TSL_MAX_TOKEN_LEN);

  context->inside_quotes = false;
  context->is_string_literal = false;

}
