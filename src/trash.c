#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifndef NDEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif // NDEBUG
static const char* trashdir = ".trash/";
int
main(int argc, char* argv[])
{
  int err = mkdir(".trash", S_IRUSR | S_IWUSR | S_IXUSR);
  if (err && errno != EEXIST) {
    printf("error: %s\n", strerror(errno));
    return 1;
  }
  if (argc <= 1) {
    printf("Usage: trash <filename>..\n");
    return 1;
  }
  int i;
  char* trashname = NULL;
  for (i = 1; i < argc; i++) {
    char* filename = strrchr(argv[i], '/');
    if (filename == NULL) {
      filename = argv[i];
    } else {
      filename += 1;
    }
    trashname = realloc(trashname, 
        sizeof(char) * (strlen(filename) + strlen(trashdir) + 1));
    if (trashname == NULL) {
      printf("error: %s\n", strerror(errno));
      return 1;
    }
    trashname[0] = '\0';
    strcat(trashname, trashdir);
    strcat(trashname, filename);
    debug("trashname = %s\n", trashname);
    if (rename(argv[i], trashname) == -1) {
      printf("error: %s\n", strerror(errno));
      return 1;
    }
  }
  free(trashname);
  return 0;
}

