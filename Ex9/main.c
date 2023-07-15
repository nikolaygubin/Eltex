#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print_proc_info(char * node) {
  printf("node : %s\nmy pid : %d\nparent : %d\n\n", node, getpid(), getppid());
}

int main() {
  pid_t l_child, r_child, l_l_child, l_r_child, r_r_child;
  int status;

  l_child = fork(); // neg - error; 0 - child; 0 > - parent;

  if (l_child > 0) {
    wait(&status);
    printf("status %d - завершён l процесс\n", WEXITSTATUS(status));

    r_child = fork();
    if (r_child > 0) {
      wait(&status);
      printf("status %d - завершён r процесс\n", WEXITSTATUS(status));
    }
    else if (r_child == 0) {
      print_proc_info("r_child");
      r_r_child = fork();
      if (r_r_child > 0) {
        wait(&status);
        printf("status %d - завершён r_r процесс\n", WEXITSTATUS(status));
      }
      else if (r_r_child == 0) {
        print_proc_info("r_r_child");
        exit(5);
      }
      exit(4);
    }
  }
  else if (l_child == 0) {
    print_proc_info("l_child");

    l_l_child = fork();
    if (l_l_child > 0) {
      wait(&status);
      printf("status %d - завершён l_l процесс\n", WEXITSTATUS(status));

      l_r_child = fork();
      if (l_r_child > 0) {
        wait(&status);
        printf("status %d - завершён l_r процесс\n", WEXITSTATUS(status));
      }
      else if (l_r_child == 0) {
        print_proc_info("l_r_child");
        exit(3);
      }
    }
    else if (l_l_child == 0) {
      print_proc_info("l_l_child");
      exit(2);
    }
    exit(1);
  }

  return 0;  
}