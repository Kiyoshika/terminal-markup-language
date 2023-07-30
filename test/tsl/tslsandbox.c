#include "parser.h"

int main()
{
  tsl_parser_parse("if (x == y || x==y) {}");
  return 0;
}
