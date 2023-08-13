#include "parser.h"
#include "parser_internal.h"
#include "parser_actions.h"
#include "parser_instructions.h"

// NOTE: error handling isn't imlemented yet, we just have dummy messages & exits for now

bool
tsl_parser_actions_datatype(
  struct parse_context_t* const context)
{
  if (context->current_state != TSL_STATE_NONE
      && context->current_state != TSL_STATE_ADD_FUNCTION_ARG)
  {
    printf("invalid state.\n");
    return false;
  }

  // by default, we'll assume we're parsing a variable unless we run into a TSL_TOKEN_OPEN_PAREN
  if (context->current_state == TSL_STATE_NONE)
    context->current_state = TSL_STATE_CREATING_VAR;
  // has no effect, but just here to make it explicit that we're keeping it in the function arg state
  else
    context->current_state = TSL_STATE_ADD_FUNCTION_ARG;
  context->datatype = context->current_token;

  return true;
}

bool
tsl_parser_actions_text(
  struct parse_context_t* const context)
{
  if ((context->current_state 
        & (TSL_STATE_CREATING_VAR | TSL_STATE_CREATING_FUNCTION | TSL_STATE_ADD_FUNCTION_ARG | TSL_STATE_CREATING_RETURN_VALUE)) == 0)
  {
    printf("invalid state.\n");
    return false;
  }

  if ((context->current_state & (TSL_STATE_CREATING_VAR | TSL_STATE_ADD_FUNCTION_ARG)) > 0 && !context->assigning_value)
    strncat(context->object_name, context->current_token_text, TSL_MAX_TOKEN_LEN);
  else if ((context->current_state & (TSL_STATE_CREATING_VAR | TSL_STATE_CREATING_RETURN_VALUE)) && context->assigning_value)
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

  if (context->assigning_value && context->inside_quotes)
    strncat(context->object_value, "=", TSL_MAX_TOKEN_LEN);
  else
    context->assigning_value = true;
  
  return true;
}

bool
tsl_parser_actions_minus(
  struct parse_context_t* const context)
{
  if ((context->current_state & (TSL_STATE_CREATING_VAR | TSL_STATE_CREATING_RETURN_VALUE)) == 0)
  {
    printf("invalid state.\n");
    return false;
  }

  if (context->assigning_value)
    strncat(context->object_value, "-", TSL_MAX_TOKEN_LEN);
  
  return true;
}

bool
tsl_parser_actions_semicolon(
  struct parse_context_t* const context)
{
  bool success = false;
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, ";", TSL_MAX_TOKEN_LEN);
    success = true;
  }
  else
  {
    switch (context->current_state)
    {
      case TSL_STATE_CREATING_VAR:
        success = tsl_parser_instructions_create_var(context);
        break;

      case TSL_STATE_CREATING_RETURN_VALUE:
        success = tsl_parser_instructions_create_return_value(context);
        break;
    }

    tsl_parser_reset_state(context);
  }

  return success;
}

bool
tsl_parser_actions_quote(
  struct parse_context_t* const context)
{
  context->inside_quotes = !context->inside_quotes;
  if (context->inside_quotes)
    context->is_string_literal = true;
  return true;
}

bool
tsl_parser_actions_open_paren(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, "(", TSL_MAX_TOKEN_LEN);
    return true;
  }

  // parser starts off assuming we're creating a variable until we hit a '(' token
  if ((context->current_state & (TSL_STATE_NONE | TSL_STATE_CREATING_VAR)) == 0)
  {
    printf("invalid state.\n");
    return false;
  }

  switch (context->current_state)
  {
    case TSL_STATE_CREATING_VAR:
    {
      tsl_parser_instructions_create_function(context);
      context->current_state = TSL_STATE_ADD_FUNCTION_ARG;
      memset(context->object_name, 0, TSL_MAX_TOKEN_LEN);
      return true;
    }

    case TSL_STATE_NONE:
    {
      context->current_state = TSL_STATE_FUNCTION_CALL;
      memset(context->object_name, 0, TSL_MAX_TOKEN_LEN);
      return true;
    }

    default:
      return true;
  }
  return true;
}

bool
tsl_parser_actions_close_paren(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, ")", TSL_MAX_TOKEN_LEN);
    return true;
  }

  if ((context->current_state & TSL_STATE_ADD_FUNCTION_ARG) == 0)
  {
    printf("invalid state.\n");
    return false;
  }

  if (strlen(context->object_name) > 0)
    if (!tsl_parser_instructions_add_function_arg(context))
      return false;

  context->current_state = TSL_STATE_CREATING_FUNCTION_BODY;
  return true;
}

bool
tsl_parser_actions_open_body(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, "{", TSL_MAX_TOKEN_LEN);
    return true;
  }

  return true;
}

bool
tsl_parser_actions_close_body(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, "}", TSL_MAX_TOKEN_LEN);
    return true;
  }

  context->current_state = TSL_STATE_NONE;
  return true;
}

bool
tsl_parser_actions_comma(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, ",", TSL_MAX_TOKEN_LEN);
    return true;
  }

  if (context->current_state != TSL_STATE_ADD_FUNCTION_ARG)
  {
    printf("invalid state.\n");
    return false;
  }

  if (!tsl_parser_instructions_add_function_arg(context))
    return false;

  memset(context->object_name, 0, TSL_MAX_TOKEN_LEN);
  context->datatype = TSL_TOKEN_NONE;

  return true;
}

bool
tsl_parser_actions_return(
  struct parse_context_t* const context)
{
  if (context->assigning_value && context->inside_quotes)
  {
    strncat(context->object_value, "return", TSL_MAX_TOKEN_LEN);
    return true;
  }

  if (context->current_state != TSL_STATE_CREATING_FUNCTION_BODY)
  {
    printf("invalid state.\n");
    return false;
  }

  context->current_state = TSL_STATE_CREATING_RETURN_VALUE;
  context->assigning_value = true;
  return true;
}
