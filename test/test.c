#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#define printerror() printf("error: %s\n", strerror(errno))
int
main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;
  pid_t pid = fork();
  if (pid == -1) {
    printerror();
    return 1;
  } else if (pid == 0) {
    mkdir("junk", S_IWUSR);
    char* const trashargv[] = {"processname", "junk", NULL};
    execv("./bin/trash", trashargv);
  } else {
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
      printerror();
    }
    /* check that the junk is in the trash */
    if (rmdir(".trash/junk") == -1) {
      printerror();
      printf("test failed: the junk is not in the trash\n");
      return 1;
    }
  }
  return 0;
}
