#include "parser.h"
#include "parser_internal.h"
#include "parser_actions.h"
#include "parser_instructions.h"

// NOTE: error handling isn't imlemented yet, we just have dummy messages & exits for now

bool
tsl_parser_actions_datatype(
  struct parse_context_t* const context)
{
  if (context->current_state != TSL_STATE_NONE)
  {
    printf("invalid state.\n");
    return false;
  }

  // by default, we'll assume we're parsing a variable unless we run into a TSL_TOKEN_OPEN_PAREN
  context->current_state = TSL_STATE_CREATING_VAR;
  context->datatype = context->current_token;

  return true;
}

bool
tsl_parser_actions_text(
  struct parse_context_t* const context)
{
  if ((context->current_state & (TSL_STATE_CREATING_VAR | TSL_STATE_CREATING_FUNCTION)) == 0)
  {
    printf("invalid state.\n");
    return false;
  }

  if (context->current_state == TSL_STATE_CREATING_VAR && !context->assigning_value)
    strncat(context->object_name, context->current_token_text, TSL_MAX_TOKEN_LEN);
  else if (context->current_state == TSL_STATE_CREATING_VAR && context->assigning_value)
    strncat(context->object_value, context->current_token_text, TSL_MAX_TOKEN_LEN);

  return true;
}

bool
tsl_parser_actions_assign(
  struct parse_context_t* const context)
{
  if (context->current_state != TSL_STATE_CREATING_VAR)
  {
    printf("invalid state.\n");
    return false;
  }

  context->assigning_value = true;
  
  return true;
}

bool
tsl_parser_actions_minus(
  struct parse_context_t* const context)
{
  if (context->current_state != TSL_STATE_CREATING_VAR)
  {
    printf("invalid state.\n");
    return false;
  }

  strncat(context->object_value, context->current_token_text, TSL_MAX_TOKEN_LEN);
  
  return true;
}

bool
tsl_parser_actions_semicolon(
  struct parse_context_t* const context)
{
  switch (context->current_state)
  {
    case TSL_STATE_CREATING_VAR:
      return tsl_parser_instructions_create_var(context);
      break;
  }

  tsl_parser_reset_state(context);

  return true;
}
