#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Custom handler for SIGUSR1
void handle_sigusr1(int sig) { printf("Received SIGUSR1 signal: %d\n", sig); }

// Custom handler for SIGUSR2
void handle_sigusr2(int sig) { printf("Received SIGUSR2 signal: %d\n", sig); }

// Custom handler for SIGINT
void handle_sigint(int sig) { exit(1); }

int main() {
  signal(SIGUSR1, handle_sigusr1);
  signal(SIGUSR1, handle_sigusr2);
  signal(SIGINT, handle_sigint);

  // Infinite loop to keep the process alive to receive signals
  while (1) {
    printf("Waiting for signals...\n");
    sleep(5);
  }

  return 0;
}
