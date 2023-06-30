#include "ast.h"

struct ast_t*
ast_create(
  const enum ast_node_type_e type,
  struct ast_t* parent)
{
  if (parent && (parent->allowed_children_nodes & type) == 0)
    return (void*)0x1;

  struct ast_t* node = malloc(sizeof(*node));
  if (!node)
    return NULL;

  node->parent = parent;
  node->type = type; 
  node->body = NULL;

  switch (node->type)
  {
    case TML_NODE_ROOT:
    {
      node->contains_body = false;
      node->allowed_children_nodes = TML_NODE_TEXT;
      break;
    }
    case TML_NODE_TEXT:
    {
      node->contains_body = true;
      node->allowed_children_nodes = TML_NODE_NONE;
      break;
    }
    case TML_NODE_NONE:
      break;
  }

  node->n_children = 0;
  node->children_capacity = 1;
  node->children_types = calloc(node->children_capacity, sizeof(*node->children_types));
  node->children = calloc(node->children_capacity, sizeof(*node->children));

  node->n_attributes = 0;
  node->attribute_capacity = 1;
  node->attributes = calloc(node->attribute_capacity, sizeof(*node->attributes));

  if (!node->children_types || !node->children || !node->attributes)
  {
    free(node->children_types);
    free(node->children);
    free(node->attributes);
    free(node);
    return NULL;
  }

  return node;
}

bool
ast_add_attribute(
  struct ast_t* node,
  const enum ast_attribute_type_e type,
  const enum ast_attribute_value_e value)
{
  struct ast_attribute_pair_t attribute = {
    .type = type,
    .value = value
  };

  memcpy(&node->attributes[node->n_attributes++], &attribute, sizeof(attribute));
  
  if (node->n_attributes == node->attribute_capacity)
  {
    size_t new_capacity = node->attribute_capacity *= 2;
    void* alloc = realloc(node->attributes, new_capacity * sizeof(*node->attributes));
    if (!alloc)
      return false;
    node->attributes = alloc;
    node->attribute_capacity = new_capacity;
  }

  return true;
}

void
ast_free(
  struct ast_t** root)
{
  if (!*root)
    return;

  for (size_t i = 0; i < (*root)->n_children; ++i)
  {
    struct ast_t* child = (*root)->children[i];
    ast_free(&child);
  }

  free((*root)->children);
  (*root)->children = NULL;

  free((*root)->children_types);
  (*root)->children_types = NULL;

  free((*root)->attributes);
  (*root)->attributes = NULL;

  free((*root)->body);
  (*root)->body = NULL;

  free(*root);
  *root = NULL;
}

bool
ast_add_child(
  struct ast_t* node,
  const struct ast_t* const child)
{
  node->children_types[node->n_children] = child->type;
  node->children[node->n_children] = child;
  node->n_children++;

  if (node->n_children == node->children_capacity)
  {
    size_t new_capacity = node->children_capacity * 2;

    void* alloc_types = realloc(node->children_types, new_capacity * sizeof(*node->children_types));
    if (!alloc_types)
      return false;
    node->children_types = alloc_types;

    void* alloc = realloc(node->children, new_capacity * sizeof(void*));
    if (!alloc)
      return false;
    node->children = alloc;

    node->children_capacity = new_capacity;
  }

  return true;
}

bool
ast_add_body(
  struct ast_t* node,
  const char* const body)
{
  char* _body = strdup(body);
  if (!_body)
    return false;

  node->body = _body;
  return true;
}

// these colour IDs must match ncurses (e.g., COLOR_WHITE, etc)
size_t
ast_get_colour_id(
  const enum ast_attribute_value_e value)
{
  switch (value)
  {
    case TML_ATTRIBUTE_VALUE_BLACK:
      return 0;
    case TML_ATTRIBUTE_VALUE_RED:
      return 1;
    case TML_ATTRIBUTE_VALUE_GREEN:
      return 2;
    case TML_ATTRIBUTE_VALUE_YELLOW:
      return 3;
    case TML_ATTRIBUTE_VALUE_BLUE:
      return 4;
    case TML_ATTRIBUTE_VALUE_MAGENTA:
      return 5;
    case TML_ATTRIBUTE_VALUE_CYAN:
      return 6;
    case TML_ATTRIBUTE_VALUE_WHITE:
      return 7;
    case TML_ATTRIBUTE_VALUE_TRUE:
    case TML_ATTRIBUTE_VALUE_FALSE:
    case TML_ATTRIBUTE_VALUE_NONE:
      return 0;
  }

  return 0;
}

void
ast_render(
  const struct ast_t* const root)
{
  const size_t n_colours = 8;

  // ncurses setup
  initscr();
  if (has_colors())
  {
    // this initialises every colour combination according
    // to ncurses' colour IDs (e.g., COLOR_WHITE).
    //
    // our attribute colours are mapped to these IDs with
    // ast_get_colour_id()
    start_color();
    for (size_t i = 0; i < n_colours; ++i)
      for (size_t k = 0; k < n_colours; ++k)
        init_pair(i * n_colours + k, i, k);
  }

  size_t current_y = 0;
  size_t current_x = 0;

  // default foreground/background
  size_t fg = ast_get_colour_id(TML_ATTRIBUTE_VALUE_WHITE);
  size_t bg = ast_get_colour_id(TML_ATTRIBUTE_VALUE_BLACK);

  // get new fg/bg from root node (if any)
  for (size_t i = 0; i < root->n_attributes; ++i)
  {
    switch (root->attributes[i].type)
    {
      case TML_ATTRIBUTE_FOREGROUND:
        fg = ast_get_colour_id(root->attributes[i].value);
        break;

      case TML_ATTRIBUTE_BACKGROUND:
        bg = ast_get_colour_id(root->attributes[i].value);
        break;

      case TML_ATTRIBUTE_NEWLINE:
      case TML_ATTRIBUTE_BOLD:
      case TML_ATTRIBUTE_NULL:
        break;
    }
  }

  // set entire terminal fg/bg determined from root node
  attron(A_BOLD);
  wbkgd(stdscr, COLOR_PAIR(fg * n_colours + bg));
  attroff(A_BOLD);

  for (size_t i = 0; i < root->n_children; ++i)
  {
    struct ast_t* child = root->children[i];

    /* setup child attributes before rendering */
    bool change_colour = false;
    size_t temp_fg = fg;
    size_t temp_bg = bg;

    for (size_t attr = 0; attr < child->n_attributes; ++attr)
    {
      switch (child->attributes[attr].type)
      {
        case TML_ATTRIBUTE_FOREGROUND:
        {
          change_colour = true;
          temp_fg = ast_get_colour_id(child->attributes[attr].value);
          break;
        }

        case TML_ATTRIBUTE_BACKGROUND:
        {
          change_colour = true;
          temp_bg = ast_get_colour_id(child->attributes[attr].value);
          break;
        }

        case TML_ATTRIBUTE_NEWLINE:
        case TML_ATTRIBUTE_BOLD:
        case TML_ATTRIBUTE_NULL:
          break;
      }
    }

    if (change_colour)
      attrset(COLOR_PAIR(temp_fg * n_colours + temp_bg));
    else
      attrset(COLOR_PAIR(fg * n_colours + bg));

    /* render content */
    switch (child->type)
    {
      case TML_NODE_TEXT:
      {
        move(current_y, current_x);
        printw("%s", child->body);
        current_y++;
        break;
      }

      case TML_NODE_NONE:
      case TML_NODE_ROOT:
        break;
    }
  }
  getch();
  endwin();
}
