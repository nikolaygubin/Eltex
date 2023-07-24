#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void signal_SIGUSR1() {
  printf("Поступил сигнал SIGUSR1 !\n");
}
 
int main() {
  // signal(SIGUSR1, signal_SIGUSR1);
  struct sigaction act, oldact;
  sigset_t set, oldset;

  sigemptyset(&set);
  sigdelset(&set, SIGUSR1);
  sigdelset(&set, SIGSTOP);
  sigpending(&set);
  sigprocmask(SIG_SETMASK, &set, &oldset);

  act.sa_handler = signal_SIGUSR1;
  act.sa_flags = SA_NOCLDSTOP;
  sigaction(SIGUSR1, &act, &oldact);  
  
  pause();
  sleep(3);

  exit(EXIT_SUCCESS);
}