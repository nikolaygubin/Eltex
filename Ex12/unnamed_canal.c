#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int parent_to_child_fd[2], child_to_parent_fd[2];
  char *buf = malloc(20);

  pid_t child_pid;

  if (pipe(parent_to_child_fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (pipe(child_to_parent_fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  write(parent_to_child_fd[1], "hello, child", strlen("hello, child"));

  switch (child_pid = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
      break;

    case 0:
      if (close(parent_to_child_fd[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if ((close(child_to_parent_fd[0])) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }

      read(parent_to_child_fd[0], buf, 20);
      printf("%s\n", buf);
      write(child_to_parent_fd[1], "hello, father", strlen("hello, father"));

      if (close(parent_to_child_fd[0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if (close(child_to_parent_fd[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
      break;

    default:
      wait(NULL);
      if (close(parent_to_child_fd[0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if (close(child_to_parent_fd[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }

      read(child_to_parent_fd[0], buf, 20);
      printf("%s\n", buf);
      free(buf);

      if (close(parent_to_child_fd[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if (close(child_to_parent_fd[0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      break;
  }
  exit(EXIT_SUCCESS);
}
