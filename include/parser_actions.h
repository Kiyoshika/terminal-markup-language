#ifndef PARSER_ACTIONS_H
#define PARSER_ACTIONS_H

#include <stdbool.h>

// forward declarations
struct parse_context_t;

bool
_parser_actions_open_tag(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_text(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_space(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_text(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_equals(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_close_tag(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

bool
_parser_actions_slash(
  struct parse_context_t* context,
  const char current_char,
  struct ast_t** current_node,
  char* err_msg);

#endif
