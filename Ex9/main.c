#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t l_child, r_child, l_l_child, l_r_child, r_r_child;
  int status;

  l_child = fork();
  r_child = fork();
  if (l_child == 0) {
    printf("left child id: %d\n", getpid());
    exit(10);

    l_l_child = fork();
    l_r_child = fork();
    
    if (l_l_child == 0) {
      printf("left left child id: %d\n", getpid());
      exit(10);
    } else {
      wait(&status);
      printf("left child id: %d\n", getpid());
    }

    if (l_r_child == 0) {
      printf("left right child id: %d\n", getpid());
      exit(10);
    } else {
      wait(&status);
      printf("left child id: %d\n", getpid());
    }
  } else {
    wait(&status);
    printf("parent id: %d\n", getpid());
  }

  if (r_child == 0) {
    printf("right child id: %d\n", getpid());
    exit(10);
    r_r_child = fork();
    if (r_r_child == 0) {
      printf("right right child id: %d\n", getpid());
      exit(10);
    } else {
      wait(&status);
      printf("right child id:  %d\n", getpid());
    }
  } else {
    wait(&status);
    printf("parent id: %d\n", getpid());
  }
}
