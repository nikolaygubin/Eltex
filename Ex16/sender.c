#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main() {
  pid_t pid_receiver;
  printf("Введите PID процесса: ");
  scanf("%d", &pid_receiver);

  printf("via 5 second receiver will closed!\n");
  sleep(5);
  if (kill(pid_receiver, SIGUSR1) == -1) {
    perror("kill");
  }

  exit(EXIT_SUCCESS);
}