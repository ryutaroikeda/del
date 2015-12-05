/* broken, do not use */
#include <errno.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifndef NDEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif // NDEBUG
int
main(int argc, char* argv[])
{
  char* home = getenv("HOME");
  if (home == NULL) {
    printf("error 0: Set the environment HOME to the home directory\n");
    return 1;
  }
  const char* trashcan = "/.trashcan/";
  size_t size = strlen(home) + strlen(trashcan) + 1;
  char* trashdir = malloc(size);
  snprintf(trashdir, size, "%s%s", home, trashcan);
  errno = 0;
  int err = mkdir(trashdir, S_IRUSR | S_IWUSR | S_IXUSR);
  if (err && errno != EEXIST) {
    printf("error 1: %s %s\n", strerror(errno), trashdir);
    return 1;
  }
  if (argc <= 1) {
    printf("Usage: trash <filenames>\n");
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
    size_t bufsize = strlen(filename) + strlen(trashdir) + 1 + 500;
    trashname = realloc(trashname, bufsize);
    if (trashname == NULL) {
      printf("error 3: %s\n", strerror(errno));
      return 1;
    }
    snprintf(trashname, bufsize, "%s%s", trashdir, filename);
    // Check if file/directory trashname exists. If so, append numbers 
    // until a fresh name is found.
    struct stat s;
    uint32_t num = 0;
    while (lstat(trashname, &s) == 0) {
      snprintf(trashname, bufsize, "%s%s%"PRIu32, trashdir, filename, num);
      num++;
      if (num == 0) {
        printf("error 4: Could not rename file %s. "
             "Try emptying the trash first.\n", filename);
        return 1;
      }
    }
    debug("trashname = %s\n", trashname);
    if (rename(filename, trashname) == -1) {
      printf("error 5: %s\n", strerror(errno));
      return 1;
    }
  }
  free(trashdir);
  free(trashname);
  return 0;
}

