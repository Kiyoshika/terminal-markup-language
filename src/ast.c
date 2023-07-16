#include "ast.h"
#include "iarray.h"
#include <ctype.h>

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

  node->body = (struct ast_string_body_t){
    .content = calloc(10, sizeof(char)),
    .length = 0,
    .capacity = 10
  };

  if (!node->body.content)
  {
    free(node);
    return NULL;
  }

  switch (node->type)
  {
    case TML_NODE_ROOT:
    {
      node->contains_body = false;
      node->allowed_children_nodes = TML_NODE_TEXT | TML_NODE_SPACE | TML_NODE_INPUT;
      break;
    }
    case TML_NODE_TEXT:
    {
      node->contains_body = true;
      node->allowed_children_nodes = TML_NODE_NONE;
      break;
    }
    case TML_NODE_SPACE:
    {
      node->contains_body = false;
      node->allowed_children_nodes = TML_NODE_NONE;
      break;
    }
    case TML_NODE_INPUT:
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
  const enum ast_attribute_value_e value,
  const char* custom_value)
{
  struct ast_attribute_pair_t attribute = {
    .type = type,
    .value = value,
    .custom_value = custom_value
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

  free((*root)->body.content);
  (*root)->body.content = NULL;

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
  size_t len = strlen(body);
  size_t capacity = len * 2;
  char* _body = calloc(capacity, sizeof(char));
  if (!_body)
    return false;
  strncat(_body, body, len);

  free(node->body.content);
  node->body = (struct ast_string_body_t){
    .content = _body,
    .length = len,
    .capacity = capacity
  };

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
    case TML_ATTRIBUTE_VALUE_CUSTOM:
      return 0;
  }

  return 0;
}

/* TODO: this function is growing very large and can use some cleanup */
void
ast_draw(
  const struct ast_t* const root,
  struct iarray_t* const interactive_items)
{
  clear();
  const size_t n_colours = 8;

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

      // no-ops, just here to prevent compiler warnings
      case TML_ATTRIBUTE_NEWLINE:
      case TML_ATTRIBUTE_BOLD:
      case TML_ATTRIBUTE_NULL:
      case TML_ATTRIBUTE_MINLENGTH:
      case TML_ATTRIBUTE_MAXLENGTH:
      case TML_ATTRIBUTE_CALLBACK:
        break;
    }
  }

  // set entire terminal fg/bg determined from root node
  attron(A_BOLD);
  wbkgd(stdscr, COLOR_PAIR(fg * n_colours + bg));
  attroff(A_BOLD);

  bool is_bold = false;
  bool is_newline = true;

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
          is_newline = child->attributes[attr].value == TML_ATTRIBUTE_VALUE_TRUE;
          break;
        case TML_ATTRIBUTE_BOLD:
         is_bold = child->attributes[attr].value == TML_ATTRIBUTE_VALUE_TRUE;
         break;
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
        if (is_bold)
          attron(A_BOLD);
        // if length is 0, print nothing, otherwise it prints "(null)"
        if (child->body.length > 0)
          printw("%s", child->body.content);
        attroff(A_BOLD);
        if (is_newline)
        {
          current_y++;
          current_x = 0;
        }
        else
          current_x += child->body.length;
        break;
      }

      case TML_NODE_SPACE:
      {
        move(current_y, current_x);
        printw(" ");
        current_x++;
        break;
      }

      case TML_NODE_INPUT:
      {
        size_t body_len = 0;
        if (child->contains_body)
          body_len = child->body.length;

        iarray_add(interactive_items, child, current_x, current_y, body_len + 1);
        move(current_y, current_x);
        printw("[");
        current_x++;
        if (body_len > 0)
        {
          printw("%s", child->body.content);
          current_x += body_len; 
        }
        printw("]");
        current_x++;

        if (is_newline)
        {
          current_y++;
          current_x = 0;
        }
        break;
      }

      case TML_NODE_NONE:
      case TML_NODE_ROOT:
        break;
    }

    // reset to defaults
    is_bold = false;
    is_newline = true;
  }
}

void
ast_insert_char_to_body(
  struct ast_t* const ast,
  const char c,
  const size_t position)
{
  size_t i = ast->body.length + 1;
  while (i > 1 && i --> position)
    ast->body.content[i] = ast->body.content[i - 1];
  ast->body.content[position] = c;
  ast->body.length++;

  if (ast->body.length == ast->body.capacity)
  {
    size_t new_capacity = ast->body.capacity * 2;
    void* alloc = realloc(ast->body.content, new_capacity);
    if (!alloc)
      return;
    ast->body.content = alloc;
    ast->body.capacity = new_capacity;
    memset(&ast->body.content[ast->body.length], 0, ast->body.capacity - ast->body.length);
  }
}

void
ast_remove_char_from_body(
  struct ast_t* const ast,
  const size_t position)
{
  for (size_t i = position; i < ast->body.length; ++i)
    ast->body.content[i] = ast->body.content[i + 1];
  ast->body.content[ast->body.length - 1] = '\0';
  ast->body.length--;
}

void
ast_render(
  const struct ast_t* const root)
{
  struct iarray_t* interactive_items = iarray_create();
  if (!interactive_items)
    return;

  const int KEY_ESC = 27; // also technically ALT but we don't need that key
  int current_key = 0;
  mmask_t old;
  struct iarray_item_t* clicked_item = NULL;
  size_t mouse_x = 0;
  size_t mouse_y = 0;

  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old);

  curs_set(0); // hide cursor (unfocused on an element)

  ast_draw(root, interactive_items);

  while ((current_key = getch()) != KEY_ESC)
  {
    if (current_key == KEY_MOUSE)
    {
      MEVENT event;
      getmouse(&event);
      mouse_x = event.x;
      mouse_y = event.y;
      clicked_item = iarray_find(interactive_items, mouse_x, mouse_y);
      if (clicked_item)
      {
        curs_set(1); // show cursor (focused on an element)
        move(mouse_y, mouse_x);
        refresh();
      }
      else
        curs_set(0); // hide cursor (unfocused on an element)
    }
    else if (current_key == KEY_BACKSPACE)
    {
      if (clicked_item
          && clicked_item->node->type == TML_NODE_INPUT)
      {
        // do not backspace if the content is empty
        if (clicked_item->node->body.length == 0)
          continue;

        // do not backspace if we're at the beginning of the input
        if (mouse_x == clicked_item->x)
          continue;

        size_t position = mouse_x - clicked_item->x;
        if (position > 0)
          position--;
        ast_remove_char_from_body(clicked_item->node, position);
        clicked_item->width--;
        iarray_shift_x_left(interactive_items, clicked_item->x, mouse_y, 1);
        ast_draw(root, interactive_items);
        mouse_x--;
        move(mouse_y, mouse_x);
        refresh();
      }
    }
    else
    {
      if (clicked_item 
          && clicked_item->node->type == TML_NODE_INPUT
          && (isalnum(current_key) || ispunct(current_key) || isdigit(current_key) || current_key == ' '))
      {
        const size_t position = mouse_x - clicked_item->x;
        ast_insert_char_to_body(clicked_item->node, (const char)current_key, position);
        clicked_item->width++;
        iarray_shift_x_right(interactive_items, clicked_item->x, mouse_y, 1);
        ast_draw(root, interactive_items);
        mouse_x++;
        move(mouse_y, mouse_x);
        refresh();
      }
    }
  }
  endwin();
  iarray_free(&interactive_items);
}
