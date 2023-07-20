#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char command1[20], command2[20], arg1[20], arg2[20], buf[100];
  int fd[2];
  pid_t process_com1, process_com2;

  printf("Enter command  type : COM1 ARG1 | COM2 ARG2\n");
  fgets(buf, 100, stdin);
  sscanf(buf, "%s %s | %s %s\n", command1, arg1, command2, arg2);

  if (pipe(fd)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  switch (process_com1 = fork()) {
    case -1:
      perror("fork 1");
      exit(EXIT_FAILURE);

    case 0:  // child
      if (close(fd[0]) == -1) {
        perror("close 1");
        exit(EXIT_FAILURE);
      }
      if (fd[1] != STDOUT_FILENO) {
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
          perror("dup2 1");
          exit(EXIT_FAILURE);
        }
        if (close(fd[1]) == -1) {
          perror("close 2");
          exit(EXIT_FAILURE);
        }
      }
      execlp(command1, command1, arg1, (char*)0);
      perror("execlp 1");
      exit(EXIT_FAILURE);

    default:  // parent
      break;
  }
  switch (process_com2 = fork()) {
    case -1:
      perror("fork 2");
      exit(EXIT_FAILURE);

    case 0:  // child
      if (close(fd[1]) == -1) {
        perror("close 3");
        exit(EXIT_FAILURE);
      }
      if (fd[0] != STDIN_FILENO) {
        if (dup2(fd[0], STDIN_FILENO) == -1) {
          perror("dup2 2");
          exit(EXIT_FAILURE);
        }
        if (close(fd[0]) == -1) {
          perror("close 4");
          exit(EXIT_FAILURE);
        }
      }
      // execlp("wc", "wc", (char*)0);
      execlp(command2, command2, arg2, (char*)0);
      perror("execlp 2");
      exit(EXIT_FAILURE);
    default:  // parent
      break;
  }
  if (close(fd[0]) == -1) {
    perror("close 5");
    exit(EXIT_FAILURE);
  }
  if (close(fd[1]) == -1) {
    perror("close 6");
    exit(EXIT_FAILURE);
  }

  if (wait(0) == -1) {
    perror("wait 1");
    exit(EXIT_FAILURE);
  }
  if (wait(0) == -1) {
    perror("wait 2");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}