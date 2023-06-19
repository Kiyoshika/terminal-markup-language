#include "filesystem.h"

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

  printf("%s\n", source_content);
  free(source_content);
  return 0;
}
