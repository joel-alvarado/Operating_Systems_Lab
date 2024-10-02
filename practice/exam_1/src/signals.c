#include <signal.h>
#include <stdio.h>

void sigusr1_handler(int signum) {
  printf("Caught SIGUSR1 signal: %d\n", signum);
}

int main() {
  signal(SIGUSR1, sigusr1_handler);  // Register signal handler

  printf("Waiting for SIGUSR1 signal...\n");
  while (1);  // Infinite loop to wait for signal

  return 0;
}
