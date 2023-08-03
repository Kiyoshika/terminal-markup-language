#include "ast.h"
#include "ast_internal.h"
#include "iarray.h"

static void
_apply_margin(
  const uint32_t margin,
  size_t* current_x,
  const struct ast_colours_t* const colours)
{
  // do not apply node-specific colours on margin,
  // use the root node's colours
  attrset(COLOR_PAIR(colours->root_fg * colours->n_colours + colours->root_bg));

  if (margin > 0)
    for (uint32_t i = 0; i < margin; ++i)
      printw(" ");
  (*current_x) += margin;

  // reset back to node-specific colours
  attrset(COLOR_PAIR(colours->node_fg * colours->n_colours + colours->node_bg));
}

void
ast_render_text(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y,
  const struct ast_colours_t* const colours)
{
  (void)interactive_items;

  _apply_margin(attributes->margin_left, current_x, colours);
  move(*current_y, *current_x);
  refresh();

  if (attributes->is_bold)
    attron(A_BOLD);
  // if length is 0, print nothing, otherwise it prints "(null)"
  if (node->body.length > 0)
    printw("%s", node->body.content);
  attroff(A_BOLD);
  if (attributes->is_newline)
  {
    (*current_y)++;
    *current_x = 0;
  }
  else
    *current_x += node->body.length;

  _apply_margin(attributes->margin_right, current_x, colours);
  move(*current_y, *current_x);
}

void
ast_render_space(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y,
  const struct ast_colours_t* const colours)
{
  (void)node;
  (void)attributes;
  (void)interactive_items;
  (void)colours;

  move(*current_y, *current_x);
  printw(" ");
  (*current_x)++;
}

void
ast_render_input(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y,
  const struct ast_colours_t* const colours)
{
  _apply_margin(attributes->margin_left, current_x, colours);
  move(*current_y, *current_x);

  size_t body_len = 0;
  if (node->contains_body)
    body_len = node->body.length;

  if (body_len > attributes->fixed_width)
    body_len = attributes->fixed_width;

  iarray_add(interactive_items, node, *current_x + 1, *current_y, attributes->fixed_width);
  move(*current_y, *current_x);

  printw("[");
  (*current_x)++;

  size_t start_x = *current_x;

  if (body_len > 0)
  {
    if (attributes->is_password)
    {
      char* tmp = calloc(body_len + 1, sizeof(char));
      if (!tmp)
        return;

      for (size_t i = 0; i < body_len; ++i)
        tmp[i] = '*';
      tmp[body_len] = '\0';
      printw("%s", tmp);
      free(tmp);
    }
    else
    {
      for (size_t i = 0; i < body_len; ++i)
        printw("%c", node->body.content[i]);
    }

    *current_x += body_len; 
  }

  size_t remaining_space = attributes->fixed_width - (*current_x - start_x);
  for (size_t i = 0; i < remaining_space; ++i)
  {
    printw(" ");
    (*current_x)++;
  }

  printw("]");
  (*current_x)++;

  if (attributes->is_newline)
  {
    (*current_y)++;
    *current_x = 0;
  }

  _apply_margin(attributes->margin_right, current_x, colours);
  move(*current_y, *current_x);
}

void
ast_render_button(
  struct ast_t* const node,
  struct ast_attributes_t* const attributes,
  struct iarray_t* const interactive_items,
  size_t* current_x,
  size_t* current_y,
  const struct ast_colours_t* const colours)
{
  _apply_margin(attributes->margin_left, current_x, colours);
  move(*current_y, *current_x);

  size_t body_len = 0;
  if (node->contains_body)
    body_len = node->body.length;

  size_t center_element_pos = attributes->fixed_width / 2;
  size_t center_text_pos = body_len / 2;
  size_t start_pos = 0;

  size_t clickable_size = attributes->fixed_width;
  if (body_len > attributes->fixed_width)
    clickable_size = body_len;
  clickable_size++; // need to extend one more pixel otherwise the last pixel isn't clickable

  if (center_element_pos > center_text_pos)
    start_pos = center_element_pos - center_text_pos;

  if (body_len > attributes->fixed_width)
    body_len = attributes->fixed_width;
  
  iarray_add(interactive_items, node, *current_x, *current_y, clickable_size); 
  move(*current_y, *current_x);

  for (size_t i = 0; i < start_pos; ++i)
  {
    printw(" ");
    (*current_x)++;
  }

  printw("(");
  (*current_x)++;

  if (node->body.length > 0)
  {
    printw("%s", node->body.content);
    *current_x += node->body.length;
  }
  printw(")");
  (*current_x)++;

  for (size_t i = 0; i < attributes->fixed_width - (start_pos + body_len); ++i)
  {
    printw(" ");
    (*current_x)++;
  }

  if (attributes->is_newline)
  {
    (*current_y)++;
    *current_x = 0;
  }

  _apply_margin(attributes->margin_right, current_x, colours);
  move(*current_y, *current_x);
}
