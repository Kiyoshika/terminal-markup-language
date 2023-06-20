#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "ast.h"

#define N_TOKEN_TYPES 2
#define N_ATTRIBUTE_TYPES 2

extern const char* tml_token_text[N_TOKEN_TYPES];
extern const char* tml_attribute_text[N_ATTRIBUTE_TYPES];

enum tml_token_type_e
{
  TML_TOKEN_NULL = 0x00,            // 0000 0000
  // <
  TML_TOKEN_OPEN_TAG = (1 << 0u),   // 0000 0001
  // >
  TML_TOKEN_CLOSE_TAG = (1 << 1u),  // 0000 0010
  // a-z, A-Z
  TML_TOKEN_TEXT = (1 << 2u),       // 0000 0100
  // =
  TML_TOKEN_EQUALS = (1 << 3u),     // 0000 1000
  // /
  TML_TOKEN_SLASH = (1 << 4u),      // 0001 0000
};

enum tml_attribute_type_e
{
  TML_ATTRIBUTE_NULL = -1,
  // fg=...
  TML_ATTRIBUTE_FOREGROUND = (1 << 0u),  // 0000 0001
  // bg=...
  TML_ATTRIBUTE_BACKGROUND = (1 << 1u),  // 0000 0010
};

enum tml_state_e
{
  TML_STATE_PARSING_TAG_NAME = (1 << 0u),        // 0000 0001
  TML_STATE_PARSING_TAG_ATTRIBUTE = (1 << 1u),   // 0000 0010
};

struct parse_context_t
{
  const char* const source_buffer;
  const size_t source_buffer_len;
  size_t source_buffer_idx;
  enum tml_token_type_e current_token;
  enum tml_token_type_e previous_token;
  enum tml_token_type_e expected_token;
};

bool
parser_read_source_file(
  const char* const filepath);

enum ast_node_type_e
parser_get_node_type(
  const char* const tag_text);

enum tml_attribute_type_e
parser_get_attribute_type(
  const char* const attribute_text);

enum tml_token_type_e
parser_get_token_type(
  const char current_char);

enum tml_token_type_e
parser_get_next_expected_token(
  const enum tml_token_type_e current_token);

void
parser_next_token(
  struct parse_context_t* context);

void
parser_consume_whitespace(
  struct parse_context_t* context);

// "main loop" of the parser.
// if it fails, writes into [err_msg].
bool
parser_parse(
  struct parse_context_t* context,
  char* err_msg);

#endif
