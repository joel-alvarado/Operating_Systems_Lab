#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "infinite_counter.h"

void handle_sigusr1(int sig) {
  printf("Process %d: Resetting...\n", getpid());
  total = 0;
}

void handle_sigusr2(int sig) {
  printf("Process %d: Reversing...\n", getpid());
  step *= -1;
}

void handle_sigterm(int sig) {
  printf("Process %d: Terminating...\n", getpid());
  exit(0);
}

int main() {
  signal(SIGUSR1, handle_sigusr1);
  signal(SIGUSR2, handle_sigusr2);
  signal(SIGTERM, handle_sigterm);
  while (1) {
    printf("Process %d: I’m counting, and my count is %d...\n", getpid(),
           total);
    total += step;
    sleep(1);
  }
  return 0;
}
