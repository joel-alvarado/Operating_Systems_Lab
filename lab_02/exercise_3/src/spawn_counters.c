#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int counter_amount = atoi(argv[1]);
  int counter_pids[counter_amount];
  for (int i = 0; i < counter_amount; i++) {
    int p = fork();
    if (p == 0) {
      char *args[] = {"./infinite_counter", NULL};
      if (execvp(args[0], args) == -1) {
        perror("execvp");
      }
    }
    counter_pids[i] = p;
  }

  printf("Counter PIDs spawned: ");
  for (int i = 0; i < counter_amount; i++) {
    printf("%d ", counter_pids[i]);
  }
};