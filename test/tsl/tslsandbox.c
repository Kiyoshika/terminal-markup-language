#include "parser.h"

int main()
{
  tsl_parser_parse("string[] split(string& inputStr, int len) { hey }");
  return 0;
}
