#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_PARENT "./buffer_parent"
#define PATH_CHILD "./buffer_child"

int main() {
  int fd;
  char buf[20];
  pid_t child_process;

  if (mkfifo(PATH_PARENT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  if (mkfifo(PATH_CHILD, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  switch (child_process = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
      break;
    case 0:
      fd = open(PATH_PARENT, O_RDONLY);
      if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
      }
      read(fd, buf, 20);
      close(fd);
      printf("%s\n", buf);

      fd = open(PATH_CHILD, O_WRONLY);
      if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
      }
      write(fd, "hello, parent", strlen("hello, parent") + 1);
      close(fd);

      exit(EXIT_SUCCESS);
      break;
    default:
      fd = open(PATH_PARENT, O_WRONLY);
      if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
      }
      write(fd, "hello, child", strlen("hello, child") + 1);
      close(fd);
      fd = open(PATH_CHILD, O_RDONLY);
      if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
      }
      read(fd, buf, 20);
      printf("%s\n", buf);
      wait(NULL); 


      unlink(PATH_PARENT);
      unlink(PATH_CHILD);
      break;
  }

  exit(EXIT_SUCCESS);
}