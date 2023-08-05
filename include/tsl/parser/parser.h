#ifndef PARSER_H
#define PARSER_H

#define TSL_MAX_TOKEN_LEN 101
#define TSL_PARSING_STATES 3
#define TSL_N_TOKENS 28

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

// forward declaration
struct global_scope_t;

enum parse_state_e
{
  TSL_STATE_NONE = 0u,
  TSL_STATE_CREATING_VAR = (1 << 0u),
  TSL_STATE_CREATING_FUNCTION = (1 << 1u),
  TSL_STATE_ADDING_FUNCTION_PARAM = (1 << 2u),
};

enum token_type_e
{
  TSL_TOKEN_TYPE_NONE = 0u,
  TSL_TOKEN_TYPE_DATATYPE = (1 << 0u),
  TSL_TOKEN_TYPE_TEXT = (1 << 1u),
  TSL_TOKEN_TYPE_OPERATOR = (1 << 2u),
  TSL_TOKEN_TYPE_BRACKET = (1 << 3u),
  TSL_TOKEN_TYPE_MISC = (1 << 4u),
};

extern char tsl_tokens[TSL_N_TOKENS][TSL_MAX_TOKEN_LEN];
enum token_e
{
  TSL_TOKEN_NONE = -1,

  // datatypes
  TSL_TOKEN_INT = 0,
  TSL_TOKEN_FLOAT,
  TSL_TOKEN_BOOL,
  TSL_TOKEN_STRING,

  // operators
  TSL_TOKEN_GREATER_THAN,
  TSL_TOKEN_GREATER_THAN_EQUAL,
  TSL_TOKEN_LESS_THAN,
  TSL_TOKEN_LESS_THAN_EQUAL,
  TSL_TOKEN_ASSIGN, // =
  TSL_TOKEN_EQUAL, // ==
  TSL_TOKEN_NOT_EQUAL,
  TSL_TOKEN_LOGICAL_NOT,
  TSL_TOKEN_LOGICAL_OR,
  TSL_TOKEN_LOGICAL_AND,
  TSL_TOKEN_PLUS,
  TSL_TOKEN_MINUS,
  TSL_TOKEN_MULTIPLY,
  TSL_TOKEN_DIVIDE,

  // brackets 
  TSL_TOKEN_OPEN_PAREN,
  TSL_TOKEN_CLOSE_PAREN,
  TSL_TOKEN_OPEN_BODY, // {
  TSL_TOKEN_CLOSE_BODY, // }
  TSL_TOKEN_OPEN_ARRAY, // [
  TSL_TOKEN_CLOSE_ARRAY, // ]
  
  // misc
  TSL_TOKEN_COMMA,
  TSL_TOKEN_SEMICOLON,
  TSL_TOKEN_QUOTE,
  TSL_TOKEN_SPACE,
};

struct parse_context_t
{
  struct tsl_global_scope_t* global_scope;

  // information about original source buffer
  const char* const buffer;
  size_t buffer_idx;
  const size_t buffer_len;

  // contextual information about current/expected state (bitmasks of enum parse_state_e)
  uint64_t current_state;
  uint64_t expected_state;

  // contextual information about current/previous token
  char current_token_text[TSL_MAX_TOKEN_LEN];
  size_t current_token_text_len;
  enum token_e current_token;
  enum token_type_e current_token_type;

  char previous_token_text[TSL_MAX_TOKEN_LEN];
  enum token_e previous_token;
  enum token_type_e previous_token_type;

  // meta information gathered during parsing process
  enum token_e datatype;
  char object_name[TSL_MAX_TOKEN_LEN];
  bool assigning_value;
  char object_value[TSL_MAX_TOKEN_LEN]; // TODO: make this heap-allocated later
};

struct tsl_global_scope_t*
tsl_parser_parse(
  const char* const tsl_script);

char
tsl_parser_peek_token(
  const struct parse_context_t* const context);

void
tsl_parser_get_next_token(
  struct parse_context_t* const context);

void
tsl_parser_get_token_tags(
  struct parse_context_t* const context);

bool
tsl_parser_perform_action(
  struct parse_context_t* const context);

void
tsl_parser_reset_state(
  struct parse_context_t* const context);

#endif
