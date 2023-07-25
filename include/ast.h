#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#define N_TOKEN_TYPES 5
#define N_ATTRIBUTE_TYPES 10
#define N_ATTRIBUTE_VALUES 12
#define TML_TAG_NAME_LEN 21
#define TML_ATTRIBUTE_NAME_LEN 21
#define TML_ATTRIBUTE_VALUE_LEN 21

// forward declarations
struct iarray_t;

enum ast_node_type_e
{
  TML_NODE_NONE = 0u,
  TML_NODE_ROOT = (1 << 0u),
  TML_NODE_TEXT = (1 << 1u),
  TML_NODE_SPACE = (1 << 2u),
  TML_NODE_INPUT = (1 << 3u),
  TML_NODE_BUTTON = (1 << 4u),
};

enum ast_attribute_type_e
{
  TML_ATTRIBUTE_NULL = 0u,
  // fg=
  TML_ATTRIBUTE_FOREGROUND = (1 << 0u),
  // bg=
  TML_ATTRIBUTE_BACKGROUND = (1 << 1u),
  // newline=
  TML_ATTRIBUTE_NEWLINE = (1 << 2u),
  // bold=
  TML_ATTRIBUTE_BOLD = (1 << 3u),
  // callback=
  TML_ATTRIBUTE_CALLBACK = (1 << 4u),
  // minLength=
  TML_ATTRIBUTE_MINLENGTH = (1 << 5u),
  // maxLength=
  TML_ATTRIBUTE_MAXLENGTH = (1 << 6u),
  // password=
  TML_ATTRIBUTE_PASSWORD = (1 << 7u),
  // marginLeft=
  TML_ATTRIBUTE_MARGINLEFT = (1 << 8u),
  // marginRight=
  TML_ATTRIBUTE_MARGINRIGHT = (1 << 9u),
};

enum ast_attribute_value_e
{
  TML_ATTRIBUTE_VALUE_NONE = 0u,
  // colours
  TML_ATTRIBUTE_VALUE_WHITE = (1 << 0u),
  TML_ATTRIBUTE_VALUE_BLACK = (1 << 1u),
  TML_ATTRIBUTE_VALUE_RED = (1 << 2u),
  TML_ATTRIBUTE_VALUE_BLUE = (1 << 3u),
  TML_ATTRIBUTE_VALUE_YELLOW = (1 << 4u),
  TML_ATTRIBUTE_VALUE_CYAN = (1 << 5u),
  TML_ATTRIBUTE_VALUE_GREEN = (1 << 6u),
  TML_ATTRIBUTE_VALUE_MAGENTA = (1 << 7u),
  // booleans
  TML_ATTRIBUTE_VALUE_TRUE = (1 << 8u),
  TML_ATTRIBUTE_VALUE_FALSE = (1 << 9u),
  // integer value 
  TML_ATTRIBUTE_VALUE_INT = (1 << 10u),
  // custom value (anything that doesn't match above)
  TML_ATTRIBUTE_VALUE_CUSTOM = (1 << 11u),
};

struct ast_attribute_pair_t
{
  const enum ast_attribute_type_e type;
  enum ast_attribute_value_e value;
  char custom_value[TML_ATTRIBUTE_VALUE_LEN];
};

struct ast_string_body_t
{
  char* content;
  size_t length;
  size_t capacity;
};

// struct used when rendering/drawing ast
struct ast_attributes_t
{
  bool change_colour;
  size_t fg;
  size_t bg;
  bool is_newline;
  bool is_bold;
  bool is_password;
  bool is_multiline;
  uint32_t margin_left;
  uint32_t margin_right;
};

struct ast_colours_t
{
  size_t root_fg;
  size_t root_bg;
  size_t node_fg;
  size_t node_bg;
  size_t n_colours;
};

struct ast_t
{
  enum ast_node_type_e type;
  // labels whether or not a tag could contain a body (e.g., text field)
  // this does NOT include the root tag <tml></tml>
  bool contains_body; // <tag>body</tag>
  struct ast_string_body_t body;

  struct ast_t* parent;

  uint64_t allowed_children_nodes; // a bitmap composed of ast_node_type_e
  enum ast_node_type_e* children_types;
  struct ast_t** children;
  size_t n_children;
  size_t children_capacity;

  struct ast_attribute_pair_t* attributes;
  size_t n_attributes;
  size_t attribute_capacity;
};

size_t
ast_get_colour_id(
  const enum ast_attribute_value_e);

struct ast_t*
ast_create(
  const enum ast_node_type_e type,
  struct ast_t* parent);

void
ast_init_attributes(
  struct ast_attributes_t* const attributes,
  const size_t fg,
  const size_t bg);

void
ast_set_attributes_from_node(
  const struct ast_t* const node,
  struct ast_attributes_t* const attributes);

bool
ast_add_attribute(
  struct ast_t* node,
  const enum ast_attribute_type_e type,
  const enum ast_attribute_value_e value,
  const char* custom_value);

bool
ast_add_child(
  struct ast_t* node,
  const struct ast_t* const child);

bool
ast_add_body(
  struct ast_t* node,
  const char* const body);

void
ast_free(
  struct ast_t** root);

void
ast_draw(
  const struct ast_t* const root,
  struct iarray_t* const interactive_items,
  enum ast_attribute_value_e* root_fg,
  enum ast_attribute_value_e* root_bg);

void
ast_render(
  const struct ast_t* const root);

void
ast_insert_char_to_body(
  struct ast_t* const ast,
  const char c,
  const size_t position);

void
ast_remove_char_from_body(
  struct ast_t* const ast,
  const size_t position);

void
ast_clear_body(
  struct ast_t* const ast);

struct ast_attribute_pair_t*
ast_get_attribute(
  const struct ast_t* const ast,
  const enum ast_attribute_type_e attribute_type);

enum ast_attribute_value_e
ast_get_inverse_colour(
  const enum ast_attribute_value_e colour_attribute);

uint64_t
ast_get_allowed_attribute_values(
  const enum ast_attribute_type_e type);

#endif
