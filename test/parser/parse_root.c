#include <stdio.h>
#include "parser.h"

int main()
{
  if (!parser_read_source_file("parse_root.tml"))
  {
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  return 0;
}
