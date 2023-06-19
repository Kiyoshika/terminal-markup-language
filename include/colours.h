#ifndef COLOURS_H
#define COLOURS_H

#include <stddef.h>
#include <string.h>

enum colour_e
{
  TML_UNKNOWN = -1,
  TML_BLACK = 0,
  TML_WHITE
};

#define N_COLOURS 2
extern const char* colours[N_COLOURS];

enum colour_e
tml_get_colour(
  const char* const colour);

#endif
