#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
const char* path = "/tmp/";
const char* name = "trash/";
int main(int argc, char* argv[])
{
  enum { SIZE = 256 };
  int i, j;
  char trash[SIZE];
  if (snprintf(trash, SIZE, "%s%s", path, name) >= SIZE) {
    perror("The name of trash is too long");
    return 1;
  }
  mkdir(trash, S_IRUSR | S_IWUSR | S_IXUSR);
  if (errno != 0 && errno != EEXIST) {
    perror("Could not create trash");
    return 1;
  }
  if (argc <= 1) {
    printf("Usage: del <filenames>\n");
    return 1;
  }
  errno = 0;
  for (i = 1; i < argc; i++) {
    const char* basename = strrchr(argv[i], '/'); 
    if (basename == NULL) {
      basename = argv[i];
    } else {
      basename++;
      if (*basename == '\0') {
        perror("Name cannot end with /");
        return 1;
      }
    }
    char absname[SIZE];
    if (snprintf(absname, SIZE, "%s%s", trash, basename) >= SIZE) {
      perror("Filename too long");
      return 1;
    }
    struct stat s;
    for (j = 0; j < INT_MAX; j++) {
      if (lstat(absname, &s) != 0) { break; }
      if (snprintf(absname, SIZE, "%s%s%d", trash, basename, j) >= SIZE) {
        perror("Could not rename");
        return 1;
      }
    }
    if (rename(argv[i], absname) == -1) {
      perror("Could not move");
      return 1;
    }
  }
  return 0;
}
