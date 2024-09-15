#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("UNIX Signals Demo\n");
  printf(
      "After spawning counters, you can execute `counter_handler <amount>` to "
      "handle each "
      "process\n");
  printf("How many counters would you like to spawn? ");
  int process_count;
  scanf("%d", &process_count);
  printf("\n");

  // Hack, convert int to string
  char process_count_str[32];
  sprintf(process_count_str, "%d", process_count);

  int p = fork();
  if (p == 0) {
    char *args[] = {"./spawn_counters", process_count_str, NULL};
    if (execvp(args[0], args) == -1) {
      perror("execvp");
      exit(0);
    }
  }
}