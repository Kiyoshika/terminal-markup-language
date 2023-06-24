#include <stdio.h>
#include "parser.h"

int main()
{
  char err_msg[256] = {0};
  if (!parser_read_source_file("parse_root.tml", err_msg))
  {
    fprintf(stderr, "Failed to parse.\n");
    return -1;
  }

  return 0;
}
