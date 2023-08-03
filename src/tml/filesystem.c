#include "filesystem.h"

char*
fs_read_file(
  const char* const filepath)
{
  FILE* file = fopen(filepath, "r");
  if (!file)
    return NULL;

  rewind(file);
  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  rewind(file);

  char* buffer = calloc(file_size + 1, sizeof(char));
  if (!buffer)
  {
    fclose(file);
    return NULL;
  }

  if (fread(buffer, sizeof(char), file_size, file) < file_size)
  {
    fclose(file);
    free(buffer);
    return NULL;
  }
  buffer[file_size] = '\0';

  fclose(file);
  return buffer;
}
