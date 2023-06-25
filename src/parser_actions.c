#include "parser.h"
#include "parser_internal.h"
#include "parser_actions.h"
#include "errors.h"

bool
_parser_actions_open_tag(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg)
{
  (void)current_char;

  if ((context->state & (TML_STATE_OPENING_TAG | TML_STATE_PARSING_TAG_BODY)) == 0)
  {
    tml_error_unexpected_token(err_msg, context->source_buffer_idx);
    return false;
  }

  if (context->next_token == TML_TOKEN_SLASH)
    context->is_closing_tag = true;

  // add text body to current node when hitting closing tag.
  // e.g.,: <text>hello</text>
  if (context->is_closing_tag && context->state == TML_STATE_PARSING_TAG_BODY)
  {
    ast_add_body(*current_node, context->tag_body);
    _parser_reset_body(context);
  }

  context->state = TML_STATE_PARSING_TAG_NAME;
  return true;
}

bool
_parser_actions_text(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg)
{
  (void)current_node;

  uint64_t expected_state 
    = TML_STATE_PARSING_TAG_NAME 
      | TML_STATE_PARSING_ATTRIBUTE_NAME 
      | TML_STATE_PARSING_ATTRIBUTE_VALUE
      | TML_STATE_PARSING_TAG_BODY;

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

    case TML_STATE_PARSING_TAG_BODY:
    {
      if (!_parser_append_tag_body_char(context, current_char))
        return false;
      break;
    }

    // no-op states
    case TML_STATE_OPENING_TAG:
    case TML_STATE_CLOSING_TAG:
      break;
  }

  return true;
}

bool
_parser_actions_space(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg)
{
  (void)current_char;

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

        if (!context->is_closing_tag)
        {
          *current_node = ast_create(node_type, context->reference_node);
          context->reference_node = *current_node;
          if (!*current_node)
          {
            tml_error_node_failure(err_msg);
            return false;
          }
          else if (*current_node == (void*)0x1)
          {
            tml_error_disallowed_child_type(err_msg, context->tag_name, context->source_buffer_idx);
            return false;
          }
          context->state = TML_STATE_PARSING_ATTRIBUTE_NAME;
        }
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

      if (!ast_add_attribute(*current_node, attribute_type, context->attribute_value))
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

bool
_parser_actions_equals(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg)
{
  (void)current_char;
  (void)current_node;

  if ((context->state & TML_STATE_PARSING_ATTRIBUTE_NAME) == 0)
  {
    tml_error_unexpected_token(err_msg, context->source_buffer_idx);
    return false;
  }

  context->state = TML_STATE_PARSING_ATTRIBUTE_VALUE;
  return true;
}

bool
_parser_actions_close_tag(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg)
{
  (void)current_char;

  uint64_t expected_state = TML_STATE_PARSING_TAG_NAME | TML_STATE_PARSING_ATTRIBUTE_VALUE;
  if ((context->state & expected_state) == 0)
  {
    tml_error_unexpected_token(err_msg, context->source_buffer_idx);
    return false;
  }

  // checking for </tag>
  if (context->is_closing_tag)
  {
    if (parser_get_node_type(context->tag_name) != context->reference_node->type)
    {
      tml_error_close_tag_not_matching_parent(err_msg, context->tag_name, context->source_buffer_idx);
      return false;
    }

    if (context->reference_node->parent)
      ast_add_child(context->reference_node->parent, *current_node);
    context->is_closing_tag = false;
    context->state = TML_STATE_OPENING_TAG;
    context->reference_node = context->reference_node->parent;
    _parser_reset_tag_state(context);
    return true;
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

    *current_node = ast_create(node_type, context->reference_node);
    context->reference_node = *current_node;
    if (!*current_node)
    {
      tml_error_node_failure(err_msg);
      return false;
    }
    else if (*current_node == (void*)0x1)
    {
      tml_error_disallowed_child_type(err_msg, context->tag_name, context->source_buffer_idx);
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

    if (!ast_add_attribute(*current_node, attribute_type, context->attribute_value))
    {
      tml_error_node_failure(err_msg);
      return false;
    }
  }

  // create root node if it hasn't been created already
  // (root node must be <tml> tag)
  if (!context->root_node && (*current_node)->type != TML_NODE_ROOT)
  {
    tml_error_missing_root_node(err_msg);
    return false;
  }
  else if (!context->root_node)
  {
    // transfer ownership
    context->root_node = *current_node;
    *current_node = NULL;
  }
  else
  {
    if ((*current_node)->type == TML_NODE_ROOT)
    {
      tml_error_root_already_exists(err_msg);
      return false;
    }
  }

  if (context->reference_node->contains_body)
    context->state = TML_STATE_PARSING_TAG_BODY;
  else
    context->state = TML_STATE_OPENING_TAG;

  _parser_reset_tag_state(context);
  return true;
}
