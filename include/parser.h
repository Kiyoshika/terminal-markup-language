#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "ast.h"

extern const char* tml_tag_names[N_TOKEN_TYPES];
extern const char* tml_attribute_names[N_ATTRIBUTE_TYPES];
extern const char* tml_attribute_values[N_ATTRIBUTE_VALUES];

enum tml_token_type_e
{
  TML_TOKEN_NULL = 0x00,
  // <
  TML_TOKEN_OPEN_TAG = (1 << 0u),
  // >
  TML_TOKEN_CLOSE_TAG = (1 << 1u),
  // a-z, A-Z
  TML_TOKEN_TEXT = (1 << 2u),
  // =
  TML_TOKEN_EQUALS = (1 << 3u),
  // /
  TML_TOKEN_SLASH = (1 << 4u),
  TML_TOKEN_SPACE = (1 << 5u),
  // 0-9
  TML_TOKEN_NUMBER = (1 << 6u),
  // !, @, #, $, etc.
  TML_TOKEN_PUNCTUATION = (1 << 7u),
};



enum tml_state_e
{
  TML_STATE_OPENING_TAG = (1 << 0u),
  TML_STATE_CLOSING_TAG = (1 << 1u),
  TML_STATE_PARSING_TAG_NAME = (1 << 2u),
  TML_STATE_PARSING_ATTRIBUTE_NAME = (1 << 3u),
  TML_STATE_PARSING_ATTRIBUTE_VALUE = (1 << 4u),
  // <tag>body</tag>
  TML_STATE_PARSING_TAG_BODY = (1 << 5u),
};

// manage the entire context of the parser, including an original pointer to the
// .tml source code, current/next/expected tokens, state management and the current
// name/attribute buffers.
struct parse_context_t
{
  // the root node <tml> that will contain all subnodes.
  // this is created when the <tml> tag is first parsed
  struct ast_t* root_node;

  // the current node we're pointing to in the tree.
  struct ast_t* reference_node;

  // the original source code buffer and the current pointed-to index (and total length)
  const char* const source_buffer;
  const size_t source_buffer_len;
  size_t source_buffer_idx;

  // enums for state management. peek at the current, previous and expected token
  enum tml_state_e state;
  enum tml_token_type_e current_token;
  enum tml_token_type_e next_token;
  enum tml_token_type_e previous_token;
  enum tml_token_type_e expected_token;

  // the current tag or attribute name/value. these are appended to/cleared
  // during perform_action depending on the current state of the parser
  char tag_name[TML_TAG_NAME_LEN];
  char attribute_name[TML_ATTRIBUTE_NAME_LEN];
  char attribute_value[TML_ATTRIBUTE_VALUE_LEN];
  uint64_t allowed_attribute_values;

  // lengths for the above buffers so we don't have to repeatedly call strlen()
  // to size-check when trying to prevent overflowing the buffer. If user tries
  // making a tag/value longer than the max allowed length, it will be truncated
  // and eventually a not-found or not-expected error will be thrown.
  size_t tag_name_len;
  size_t attribute_name_len;
  size_t attribute_value_len;

  // if we are currently parsing a </tag>
  bool is_closing_tag;

  // any content inside a tag where ast->contains_body is true
  // e.g., <text>hello</text>
  char* tag_body;
  size_t tag_body_len;
  size_t tag_body_capacity;
};

bool
parser_read_source_file(
  const char* const filepath,
  char* err_msg);

enum ast_node_type_e
parser_get_node_type(
  const char* const tag_text);

enum ast_attribute_type_e
parser_get_attribute_type(
  const char* const attribute_text);

enum ast_attribute_value_e
parser_get_attribute_value(
  const char* const attribute_value_text);

enum tml_token_type_e
parser_get_token_type(
  const char current_char);

enum tml_token_type_e
parser_get_next_expected_token(
  const enum tml_token_type_e current_token);

void
parser_next_token(
  struct parse_context_t* context);

// "main loop" of the parser.
// if it fails, writes into [err_msg].
bool
parser_parse(
  struct parse_context_t* context,
  char* err_msg);

bool
parser_perform_token_action(
  struct parse_context_t* context,
  char* err_msg);

#endif
