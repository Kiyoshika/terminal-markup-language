#include "parser.h"

int main()
{
  tsl_parser_parse("int x =   0;\nint y = -12;");
  return 0;
}
