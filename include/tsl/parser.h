#ifndef PARSER_H
#define PARSER_H

#define TSL_MAX_TOKEN_LEN 101
#define TSL_PARSING_STATES 3
#define TSL_N_DATATYPES 4
#define TSL_N_OPERATORS 11

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

enum parse_state_e
{
  TSL_STATE_NONE = 0u,
  TSL_STATE_CREATING_VAR = (1 << 0u),
  TSL_STATE_CREATING_FUNCTION = (1 << 1u),
  TSL_STATE_ADDING_FUNCTION_PARAM = (1 << 2u),
};

enum token_e
{
  TSL_TOKEN_NONE = 0u,
  TSL_TOKEN_DATATYPE = (1 << 0u),
  TSL_TOKEN_TEXT = (1 << 1u),
  TSL_TOKEN_OPERATOR = (1 << 2u),
  TSL_TOKEN_SEMICOLON = (1 << 3u),
};

extern char tsl_datatypes[TSL_N_DATATYPES][TSL_MAX_TOKEN_LEN];
enum token_datatype_e
{
  TSL_TOKEN_DATATYPE_INT = (1 << 0u),
  TSL_TOKEN_DATATYPE_FLOAT = (1 << 1u),
  TSL_TOKEN_DATATYPE_BOOL = (1 << 2u),
  TSL_TOKEN_DATATYPE_STRING = (1 << 3u),
};

extern char tsl_operators[TSL_N_OPERATORS][TSL_MAX_TOKEN_LEN];
enum token_operator_e
{
  TSL_TOKEN_OPERATOR_ASSIGN = (1 << 0u),
  TSL_TOKEN_OPERATOR_EQUAL = (1 << 1u),
  TSL_TOKEN_OPERATOR_NOT_EQUAL = (1 << 2u),
  TSL_TOKEN_OPERATOR_GREATER_THAN = (1 << 3u),
  TSL_TOKEN_OPERATOR_GREATER_THAN_EQUAL = (1 << 4u),
  TSL_TOKEN_OPERATOR_LESS_THAN = (1 << 5u),
  TSL_TOKEN_OPERATOR_LESS_TAN_EQUAL = (1 << 6u),
  TSL_TOKEN_OPERATOR_REFERENCE = (1 << 7u),
  TSL_TOKEN_OPERATOR_NOT = (1 << 8u),
  TSL_TOKEN_OPERATOR_LOGICAL_AND = (1 << 9u),
  TSL_TOKEN_OPERATOR_LOGICAL_OR = (1 << 10u),
};


struct parse_context_t
{
  const char* const buffer;
  size_t buffer_idx;
  const size_t buffer_len;

  uint64_t current_state;
  uint64_t expected_state;

  char current_token[TSL_MAX_TOKEN_LEN];
  size_t current_token_len;

  char previous_token[TSL_MAX_TOKEN_LEN];
};

void
tsl_parser_parse(
  const char* const tsl_script);

char
tsl_parser_peek_token(
  const struct parse_context_t* const context);

void
tsl_parser_get_next_token(
  struct parse_context_t* const context);

void
tsl_parser_get_next_expected_state(
  struct parse_context_t* const context);

#endif
