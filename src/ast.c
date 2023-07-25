#include "ast.h"
#include "ast_internal.h"
#include "iarray.h"
#include <ctype.h>

#define CTRL_BACKSPACE (0x1f & 8)

#define KEY_ESC 27
#define KEY_BACKSPACE_2 8
#define KEY_BACKSPACE_3 127

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
      node->allowed_children_nodes = TML_NODE_TEXT | TML_NODE_SPACE | TML_NODE_INPUT | TML_NODE_BUTTON;
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
    case TML_NODE_BUTTON:
    {
      node->contains_body = true;
      node->allowed_children_nodes = TML_NODE_NONE;
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

void
ast_init_attributes(
  struct ast_attributes_t* const attributes,
  const size_t fg,
  const size_t bg)
{
  attributes->fg = fg;
  attributes->bg = bg;
  attributes->is_bold = false;
  attributes->is_newline = true;
  attributes->is_password = false;
  attributes->margin_left = 0;
  attributes->margin_right = 0;
}

void
ast_set_attributes_from_node(
  const struct ast_t* const node,
  struct ast_attributes_t* const attributes)
{
  for (size_t attr = 0; attr < node->n_attributes; ++attr)
  {
    switch (node->attributes[attr].type)
    {
      case TML_ATTRIBUTE_FOREGROUND:
      {
        attributes->fg = ast_get_colour_id(node->attributes[attr].value);
        break;
      }

      case TML_ATTRIBUTE_BACKGROUND:
      {
        attributes->bg = ast_get_colour_id(node->attributes[attr].value);
        break;
      }

      case TML_ATTRIBUTE_NEWLINE:
        attributes->is_newline = node->attributes[attr].value == TML_ATTRIBUTE_VALUE_TRUE;
        break;
      case TML_ATTRIBUTE_BOLD:
       attributes->is_bold = node->attributes[attr].value == TML_ATTRIBUTE_VALUE_TRUE;
       break;
      case TML_ATTRIBUTE_PASSWORD:
        attributes->is_password = node->attributes[attr].value == TML_ATTRIBUTE_VALUE_TRUE;
        break;
      case TML_ATTRIBUTE_MARGINLEFT:
      {
        char* endptr = NULL;
        attributes->margin_left = strtoul(node->attributes[attr].custom_value, &endptr, 10);
        break;
      }
      case TML_ATTRIBUTE_MARGINRIGHT:
      {
        char* endptr = NULL;
        attributes->margin_right = strtoul(node->attributes[attr].custom_value, &endptr, 10);
        break;
      }
      case TML_ATTRIBUTE_NULL:
        break;
    }
  }
}

bool
ast_add_attribute(
  struct ast_t* node,
  const enum ast_attribute_type_e type,
  const enum ast_attribute_value_e value,
  const char* custom_value)
{

  enum ast_attribute_value_e has_custom_value = TML_ATTRIBUTE_VALUE_INT | TML_ATTRIBUTE_VALUE_CUSTOM;

  struct ast_attribute_pair_t attribute = {
    .type = type,
    .value = value,
    .custom_value = {0} 
  };

  if (value & has_custom_value)
    strncat(attribute.custom_value, custom_value, (size_t)TML_ATTRIBUTE_VALUE_LEN);

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
    case TML_ATTRIBUTE_VALUE_INT:
      return 0;
  }

  return 0;
}

void
ast_draw(
  const struct ast_t* const root,
  struct iarray_t* const interactive_items,
  enum ast_attribute_value_e* root_fg,
  enum ast_attribute_value_e* root_bg)
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
        *root_fg = root->attributes[i].value;
        break;

      case TML_ATTRIBUTE_BACKGROUND:
        bg = ast_get_colour_id(root->attributes[i].value);
        *root_bg = root->attributes[i].value;
        break;

      // no-ops, just here to prevent compiler warnings
      case TML_ATTRIBUTE_NEWLINE:
      case TML_ATTRIBUTE_BOLD:
      case TML_ATTRIBUTE_NULL:
      case TML_ATTRIBUTE_MINLENGTH:
      case TML_ATTRIBUTE_MAXLENGTH:
      case TML_ATTRIBUTE_CALLBACK:
      case TML_ATTRIBUTE_PASSWORD:
      case TML_ATTRIBUTE_MARGINLEFT:
      case TML_ATTRIBUTE_MARGINRIGHT:
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
    struct ast_attributes_t attributes;
    ast_init_attributes(&attributes, fg, bg);
    ast_set_attributes_from_node(child, &attributes);

    attrset(COLOR_PAIR(attributes.fg * n_colours + attributes.bg));

    struct ast_colours_t colours = {
      .root_fg = fg,
      .root_bg = bg,
      .node_fg = attributes.fg,
      .node_bg = attributes.bg,
      .n_colours = n_colours
    };

    /* render content */
    switch (child->type)
    {
      case TML_NODE_TEXT:
        ast_render_text(child, &attributes, interactive_items, &current_x, &current_y, &colours);
        break;

      case TML_NODE_SPACE:
        ast_render_space(child, &attributes, interactive_items, &current_x, &current_y, &colours);
        break;
      
      case TML_NODE_INPUT:
        ast_render_input(child, &attributes, interactive_items, &current_x, &current_y, &colours);
        break;

      case TML_NODE_BUTTON:
        ast_render_button(child, &attributes, interactive_items, &current_x, &current_y, &colours);
        break;
      
      case TML_NODE_NONE:
      case TML_NODE_ROOT:
        break;
    }
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
  mouseinterval(0);

  curs_set(0); // hide cursor (unfocused on an element)

  // get the new root fg/bg (if any). Otherwise NULL will be the default (white on black)
  enum ast_attribute_value_e root_fg = TML_ATTRIBUTE_VALUE_WHITE;
  enum ast_attribute_value_e root_bg = TML_ATTRIBUTE_VALUE_BLACK;
  ast_draw(root, interactive_items, &root_fg, &root_bg);

  while ((current_key = getch()) != KEY_ESC)
  {
    switch (current_key)
    {
      case KEY_MOUSE:
      {
        MEVENT event;
        getmouse(&event);
        mouse_x = event.x;
        mouse_y = event.y;
        clicked_item = iarray_find(interactive_items, mouse_x, mouse_y);
        if (clicked_item)
        {
          if (clicked_item->node->type == TML_NODE_INPUT)
          {
            curs_set(1);
            move(mouse_y, mouse_x);
            refresh();
          }

          else if (clicked_item->node->type == TML_NODE_BUTTON)
          {
            curs_set(0);
            switch (event.bstate)
            {
              case BUTTON1_RELEASED:
              case BUTTON1_PRESSED:
              {
                struct ast_attribute_pair_t* bg = ast_get_attribute(clicked_item->node, TML_ATTRIBUTE_BACKGROUND);
                if (!bg)
                {
                  ast_add_attribute(clicked_item->node, TML_ATTRIBUTE_BACKGROUND, root_bg, NULL);
                  bg = &clicked_item->node->attributes[clicked_item->node->n_attributes - 1];
                }
                bg->value = ast_get_inverse_colour(bg->value);

                struct ast_attribute_pair_t* fg = ast_get_attribute(clicked_item->node, TML_ATTRIBUTE_FOREGROUND);
                if (!fg)
                {
                  ast_add_attribute(clicked_item->node, TML_ATTRIBUTE_FOREGROUND, root_fg, NULL);
                  fg = &clicked_item->node->attributes[clicked_item->node->n_attributes - 1];
                }
                fg->value = ast_get_inverse_colour(fg->value);

                ast_draw(root, interactive_items, &root_fg, &root_bg);
                break;
              }
            }
          }
          move(mouse_y, mouse_x);
          refresh();
        }
        else
          curs_set(0);
        break;
      }

      case KEY_BACKSPACE:
      case '\b':
      case 127:
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
          ast_draw(root, interactive_items, &root_fg, &root_bg);
          mouse_x--;
          move(mouse_y, mouse_x);
          refresh();
        }
        break;
      }

      /* ctrl + r */
      case 'R' & 037:
      {
        if (clicked_item
            && clicked_item->node->type == TML_NODE_INPUT)
        {
          size_t len = clicked_item->node->body.length;
          if (len > 0)
          {
            ast_clear_body(clicked_item->node);
            iarray_shift_x_left(interactive_items, clicked_item->x, clicked_item->y, len);
            clicked_item->width -= len;
            ast_draw(root, interactive_items, &root_fg, &root_bg);
            mouse_x = clicked_item->x;
            mouse_y = clicked_item->y;
            move(clicked_item->y, clicked_item->x);
          }
        }
        break;
      }

      /* any other key */
      default:
      {
        if (clicked_item
            &&clicked_item->node->type == TML_NODE_INPUT
            && (isalnum(current_key) || ispunct(current_key) || isdigit(current_key) || current_key == ' '))
        {
          curs_set(1);
          const size_t position = mouse_x - clicked_item->x;
          ast_insert_char_to_body(clicked_item->node, (const char)current_key, position);
          clicked_item->width++;
          iarray_shift_x_right(interactive_items, clicked_item->x, mouse_y, 1);
          ast_draw(root, interactive_items, &root_fg, &root_bg);
          mouse_x++;
          move(mouse_y, mouse_x);
          refresh();
        }
        break;
      }
    }
  }
  endwin();
  iarray_free(&interactive_items);
}

struct ast_attribute_pair_t*
ast_get_attribute(
  const struct ast_t* const ast,
  const enum ast_attribute_type_e attribute_type)
{
  for (size_t i = 0; i < ast->n_attributes; ++i)
    if (ast->attributes[i].type == attribute_type)
      return &ast->attributes[i];

  return NULL;
}

enum ast_attribute_value_e
ast_get_inverse_colour(
  const enum ast_attribute_value_e colour_attribute)
{
  switch (colour_attribute)
  {
    case TML_ATTRIBUTE_VALUE_WHITE:
      return TML_ATTRIBUTE_VALUE_BLACK;

    case TML_ATTRIBUTE_VALUE_BLACK:
      return TML_ATTRIBUTE_VALUE_WHITE;

    case TML_ATTRIBUTE_VALUE_RED:
      return TML_ATTRIBUTE_VALUE_CYAN;

    case TML_ATTRIBUTE_VALUE_BLUE:
      return TML_ATTRIBUTE_VALUE_YELLOW;

    case TML_ATTRIBUTE_VALUE_YELLOW:
      return TML_ATTRIBUTE_VALUE_BLUE;

    case TML_ATTRIBUTE_VALUE_CYAN:
      return TML_ATTRIBUTE_VALUE_RED;

    case TML_ATTRIBUTE_VALUE_GREEN:
      return TML_ATTRIBUTE_VALUE_MAGENTA;

    case TML_ATTRIBUTE_VALUE_MAGENTA:
      return TML_ATTRIBUTE_VALUE_GREEN;

    // non-colour attributes
    case TML_ATTRIBUTE_VALUE_NONE:
    case TML_ATTRIBUTE_VALUE_TRUE:
    case TML_ATTRIBUTE_VALUE_FALSE:
    case TML_ATTRIBUTE_VALUE_CUSTOM:
      return TML_ATTRIBUTE_VALUE_NONE;
  }

  return TML_ATTRIBUTE_VALUE_NONE;
}

void
ast_clear_body(
  struct ast_t* const ast)
{
  memset(ast->body.content, 0, ast->body.length);
  ast->body.length = 0;
}

uint64_t
ast_get_allowed_attribute_values(
  const enum ast_attribute_type_e type)
{
  switch (type)
  {
    case TML_ATTRIBUTE_FOREGROUND:
    case TML_ATTRIBUTE_BACKGROUND:
    {
      return TML_ATTRIBUTE_VALUE_WHITE
           | TML_ATTRIBUTE_VALUE_BLACK
           | TML_ATTRIBUTE_VALUE_RED
           | TML_ATTRIBUTE_VALUE_BLUE
           | TML_ATTRIBUTE_VALUE_YELLOW
           | TML_ATTRIBUTE_VALUE_CYAN
           | TML_ATTRIBUTE_VALUE_GREEN
           | TML_ATTRIBUTE_VALUE_MAGENTA;
    }

    case TML_ATTRIBUTE_NEWLINE:
    case TML_ATTRIBUTE_BOLD:
    case TML_ATTRIBUTE_PASSWORD:
    {
      return TML_ATTRIBUTE_VALUE_TRUE
           | TML_ATTRIBUTE_VALUE_FALSE;
    }

    case TML_ATTRIBUTE_MINLENGTH:
    case TML_ATTRIBUTE_MAXLENGTH:
    case TML_ATTRIBUTE_MARGINLEFT:
    case TML_ATTRIBUTE_MARGINRIGHT:
      return TML_ATTRIBUTE_VALUE_INT;

    case TML_ATTRIBUTE_CALLBACK:
      return TML_ATTRIBUTE_VALUE_CUSTOM;

    case TML_ATTRIBUTE_NULL:
      return TML_ATTRIBUTE_VALUE_NONE;
  }

  return TML_ATTRIBUTE_VALUE_NONE;
}
