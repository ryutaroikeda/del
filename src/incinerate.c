#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define printerror() printf("error: %s\n", strerror(errno))
#ifndef NDEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif
int
deleteR(DIR* d, const char* path)
{
  errno = 0;
  while (errno == 0) {
    struct dirent* ent = readdir(d);
    if (ent == NULL) { break; }
    char* name = 
      malloc(sizeof(char) * (strlen(path) + 1 + strlen(ent->d_name) + 1));
    name[0] = '\0';
    strcat(name, path);
    strcat(name, "/");
    strcat(name, ent->d_name);
    if (ent->d_type == DT_DIR) {
      DIR* dir = opendir(name);
      if (dir == NULL) {
        debug("couldn't open directory %s\n", name);
        free(name);
        printerror();
        return -1;
      }
      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
        free(name);
        closedir(dir);
        continue;
      }
      int err = deleteR(dir, name);
      closedir(dir);
      if (err == -1) {
        free(name);
        debug("couldn't delete contents of directory %s\n", name);
        return -1;
      }
      if (rmdir(name) == -1) {
        free(name);
        debug("couldn't delete directory %s\n", name);
        printerror();
        return -1;
      }
    } else if (unlink(name) == -1) {
      debug("couldn't delete file %s\n", name);
      free(name);
      printerror();
      return -1;
    }
  }
  return 0;
}
int
main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;
  char* home = getenv("HOME");
  if (home == NULL) {
    printf("error 0: Set the environment HOME to the home directory\n");
    return 1;
  }
  const char* trashcan = "/.trashcan";
  size_t bufsize = strlen(home) + strlen(trashcan) + 1;
  char* trashdir = malloc(bufsize);
  snprintf(trashdir, bufsize, "%s%s", home, trashcan);
  DIR* d = opendir(trashdir);
  if (d == NULL) {
    printerror();
    return 1;
  }
  int err = deleteR(d, trashdir);
  closedir(d);
  if (err == -1) {
    return 1;
  }
  if (rmdir(trashdir) == -1) {
    printerror();
    return 1;
  }
  return 0;
}


