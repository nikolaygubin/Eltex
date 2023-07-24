#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
  sigset_t set, oldset;
  int sig;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
  sigpending(&set);
  sigprocmask(SIG_SETMASK, &set, &oldset);

  while (sigwait(&set, &sig))
    ;

  exit(EXIT_SUCCESS);
}