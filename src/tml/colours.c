#include "colours.h"

// IMPORTANT: this sorting must match colour_e
const char* colours[N_COLOURS] = {
  "black",
  "white"
};

enum colour_e
tml_get_colour(
  const char* const colour)
{
  for (size_t i = 0; i < (size_t)N_COLOURS; ++i)
    if (strcmp(colours[i], colour) == 0)
      return i;

  return -1;
}
