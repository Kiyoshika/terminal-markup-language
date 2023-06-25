#include "filesystem.h"
#include "parser.h"

int
main(
  int argc,
  char* argv[])
{
  if (argc == 1)
  {
    printf("Please provide a file argument.\n");
    return -1;
  }

  char* source_content = fs_read_file(argv[1]);
  if (!source_content)
  {
    printf("There was a problem reading the file.\n");
    return -1;
  }

  char err_msg[256] = {0};
  if (!parser_read_source_file(argv[1], err_msg))
    printf("%s", err_msg);

  free(source_content);
  return 0;
}
