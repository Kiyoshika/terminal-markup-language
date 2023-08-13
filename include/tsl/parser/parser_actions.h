#ifndef PARSER_ACTIONS_H
#define PARSER_ACTIONS_H

#include <stdbool.h>

// forward declarations
struct parse_context_t;

bool
tsl_parser_actions_datatype(
  struct parse_context_t* const context);

bool
tsl_parser_actions_text(
  struct parse_context_t* const context);

bool
tsl_parser_actions_assign(
  struct parse_context_t* const context);

bool
tsl_parser_actions_minus(
  struct parse_context_t* const context);

bool
tsl_parser_actions_semicolon(
  struct parse_context_t* const context);

bool
tsl_parser_actions_quote(
  struct parse_context_t* const context);

bool
tsl_parser_actions_comma(
  struct parse_context_t* const context);

bool
tsl_parser_actions_open_paren(
  struct parse_context_t* const context);

bool
tsl_parser_actions_close_paren(
  struct parse_context_t* const context);

bool
tsl_parser_actions_open_body(
  struct parse_context_t* const context);

bool
tsl_parser_actions_close_body(
  struct parse_context_t* const context);

bool
tsl_parser_actions_comma(
  struct parse_context_t* const context);

#endif
