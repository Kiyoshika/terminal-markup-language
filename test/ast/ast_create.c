#include "ast.h"
#include <stdio.h>

int main()
{
  int status = -1;

  struct ast_t* root = ast_create(NULL);
  struct ast_t* root2 = ast_create("white");
  struct ast_t* root3 = ast_create("nonsense");

  if (root->bg_colour != TML_BLACK)
  {
    fprintf(stderr, "Expected bg_colour to be TML_BLACK.\n");
    goto cleanup;
  }

  if (root2->bg_colour != TML_WHITE)
  {
    fprintf(stderr, "Expected bg_colour to be TML_WHITE.\n");
    goto cleanup;
  }

  if (root3)
  {
    fprintf(stderr, "Expected root3 to be NULL.\n");
    goto cleanup;
  }

  status = 0;
  cleanup:
  ast_free(&root);
  ast_free(&root2);
  ast_free(&root3);

  return status;
}
