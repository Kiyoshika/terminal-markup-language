#include "parser.h"
#include "parser_internal.h"
#include "ast.h"
#include "filesystem.h"
#include "errors.h"

// IMPORTANT: this must be ordered the same as
// ast_node_type_e in ast.h
const char* tml_tag_names[N_TOKEN_TYPES] = {
  "tml",
  "text"
};

// IMPORTANT: this must be ordered the same as
// ast_attribute_type_e in ast.h
const char* tml_attribute_names[N_ATTRIBUTE_TYPES] = {
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
    .root_node = NULL,
    .reference_node = NULL,
    .source_buffer = source_buffer,
    .source_buffer_len = strlen(source_buffer),
    .source_buffer_idx = 0,
    .state = TML_STATE_OPENING_TAG,
    .current_token = TML_TOKEN_NULL,
    .previous_token = TML_TOKEN_NULL,
    .expected_token = TML_TOKEN_OPEN_TAG,
    .tag_name = {0},
    .attribute_name = {0},
    .attribute_value = {0},
    .tag_name_len = 0,
    .attribute_name_len = 0,
    .attribute_value_len = 0
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
    if (strcmp(tml_tag_names[i], tag_text) == 0)
      return (1 << i);

  return TML_NODE_NONE;
}

enum ast_attribute_type_e
parser_get_attribute_type(
  const char* const attribute_text)
{
  for (size_t i = 0; i < (size_t)N_ATTRIBUTE_TYPES; ++i)
    if (strcmp(tml_attribute_names[i], attribute_text) == 0)
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
    case TML_TOKEN_TEXT:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT | TML_TOKEN_EQUALS | TML_TOKEN_CLOSE_TAG;
    case TML_TOKEN_CLOSE_TAG:
      return TML_TOKEN_SPACE | TML_TOKEN_OPEN_TAG;
    case TML_TOKEN_EQUALS:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT;
    case TML_TOKEN_SLASH:
      return TML_TOKEN_SPACE | TML_TOKEN_TEXT;
    case TML_TOKEN_NULL:
      return TML_TOKEN_SPACE | TML_TOKEN_NULL;
    case TML_TOKEN_SPACE:
      return ~0u; // any token
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
    {
      if ((context->state & TML_STATE_OPENING_TAG) == 0)
      {
        tml_error_unexpected_token(err_msg, context->source_buffer_idx);
        return false;
      }

      context->state = TML_STATE_PARSING_TAG_NAME;
      return true;
    }

    case TML_TOKEN_TEXT:
    {
      uint64_t expected_state 
        = TML_STATE_PARSING_TAG_NAME | TML_STATE_PARSING_ATTRIBUTE_NAME | TML_STATE_PARSING_ATTRIBUTE_VALUE;

      if ((context->state & expected_state) == 0)
      {
        tml_error_unexpected_token(err_msg, context->source_buffer_idx);
        return false;
      }

      switch (context->state)
      {

        case TML_STATE_PARSING_TAG_NAME:
        {
          _parser_append_tag_name_char(context, current_char);
          break;
        }

        case TML_STATE_PARSING_ATTRIBUTE_NAME:
        {
          _parser_append_attribute_name_char(context, current_char);
          break;
        }

        case TML_STATE_PARSING_ATTRIBUTE_VALUE:
        {
          _parser_append_attribute_value_char(context, current_char);
          break;
        }

        // no-op states
        case TML_STATE_OPENING_TAG:
        case TML_STATE_CLOSING_TAG:
          break;
      }

      return true;
    }

    case TML_TOKEN_SPACE:
    {
      switch (context->state)
      {
        case TML_STATE_PARSING_TAG_NAME:
        {
          if (context->tag_name_len > 0)
          {
            enum ast_node_type_e node_type = parser_get_node_type(context->tag_name);
            if (node_type == TML_NODE_NONE)
            {
              tml_error_unexpected_tag_name(err_msg, context->tag_name, context->source_buffer_idx);
              return false;
            }

            current_node = ast_create(node_type, context->reference_node);
            context->reference_node = current_node;
            if (!current_node)
            {
              tml_error_node_failure(err_msg);
              return false;
            }
            context->state = TML_STATE_PARSING_ATTRIBUTE_NAME;
          }
          return true;
        }

        case TML_STATE_PARSING_ATTRIBUTE_VALUE:
        {
          enum ast_attribute_type_e attribute_type = parser_get_attribute_type(context->attribute_name);
          if (attribute_type == TML_ATTRIBUTE_NULL)
          {
            tml_error_unexpected_attribute_name(err_msg, context->attribute_name, context->source_buffer_idx);
            return false;
          }

          if (context->attribute_value_len == 0)
          {
            tml_error_empty_attribute_value(err_msg, context->attribute_name, context->source_buffer_idx);
            return false;
          }

          if (!ast_add_attribute(current_node, attribute_type, context->attribute_value))
          {
            tml_error_node_failure(err_msg);
            return false;
          }

          _parser_reset_attribute(context);

          return true;
        }

        // do nothing with space token otherwise
        default:
          return true;
      }
    }

    case TML_TOKEN_EQUALS:
    {
      if ((context->state & TML_STATE_PARSING_ATTRIBUTE_NAME) == 0)
      {
        tml_error_unexpected_token(err_msg, context->source_buffer_idx);
        return false;
      }

      context->state = TML_STATE_PARSING_ATTRIBUTE_VALUE;
      return true;
    }

    // <tml>
    // <tml bg=black>
    case TML_TOKEN_CLOSE_TAG:
    {
      uint64_t expected_state = TML_STATE_PARSING_TAG_NAME | TML_STATE_PARSING_ATTRIBUTE_VALUE;
      if ((context->state & expected_state) == 0)
      {
        tml_error_unexpected_token(err_msg, context->source_buffer_idx);
        return false;
      }

      // if we close a tag while parsing name
      if (context->state == TML_STATE_PARSING_TAG_NAME)
      {
        if (context->tag_name_len == 0)
        {
          tml_error_empty_tag_name(err_msg, context->source_buffer_idx);
          return false;
        }

        enum ast_node_type_e node_type = parser_get_node_type(context->tag_name);
        if (node_type == TML_NODE_NONE)
        {
          tml_error_unexpected_tag_name(err_msg, context->tag_name, context->source_buffer_idx);
          return false;
        }

        current_node = ast_create(node_type, context->reference_node);
        context->reference_node = current_node;
        if (!current_node)
        {
          tml_error_node_failure(err_msg);
          return false;
        }
      }

      // if we close a tag while parsing attribute value
      else if (context->state == TML_STATE_PARSING_ATTRIBUTE_VALUE)
      {
        if (context->attribute_value_len == 0)
        {
          tml_error_empty_attribute_value(err_msg, context->attribute_name, context->source_buffer_idx);
          return false;
        }

        enum ast_attribute_type_e attribute_type = parser_get_attribute_type(context->attribute_name);
        if (attribute_type == TML_ATTRIBUTE_NULL)
        {
          tml_error_unexpected_attribute_name(err_msg, context->attribute_name, context->source_buffer_idx);
          return false;
        }

        if (!ast_add_attribute(current_node, attribute_type, context->attribute_value))
        {
          tml_error_node_failure(err_msg);
          return false;
        }
      }

      // create root node if it hasn't been created already
      // (root node must be <tml> tag)
      if (!context->root_node && current_node->type != TML_NODE_ROOT)
      {
        tml_error_missing_root_node(err_msg);
        return false;
      }
      else if (!context->root_node)
      {
        // transfer ownership
        context->root_node = current_node;
        current_node = NULL;
      }
      else
      {
        if (current_node->type == TML_NODE_ROOT)
        {
          tml_error_root_already_exists(err_msg);
          return false;
        }
        // TODO: append child node into root
      }

      context->state = TML_STATE_OPENING_TAG;
      _parser_reset_tag_state(context);
      return true;
    } 

    // no-op (shouldn't be encountered)
    case TML_TOKEN_NULL:
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
  while (context->source_buffer_idx < context->source_buffer_len)
  {
    //parser_consume_whitespace(context);
    if (context->source_buffer_idx >= context->source_buffer_len)
      break;

    context->current_token 
      = parser_get_token_type(context->source_buffer[context->source_buffer_idx]);

    if ((context->current_token & context->expected_token) == 0)
    {
      tml_error_unexpected_token(err_msg, context->source_buffer_idx);
      return false;
    }

    if (!parser_perform_token_action(context, err_msg))
      return false;

    parser_next_token(context);
  }

  return true;
}
