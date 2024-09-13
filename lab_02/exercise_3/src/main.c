#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("UNIX Signals Demo\n");
  printf("How many processes would you like to spawn? ");
  int process_count;
  scanf("%d", &process_count);
  printf("\n");

  char process_count_str[32];
  sprintf(process_count_str, "%d", process_count);

  int p = fork();
  if (p == 0) {
    char *args[] = {"./spawn_counters", process_count_str, NULL};
    int err = execvp(args[0], args);
    if (err == -1) {
      perror("execvp");
      exit(0);
    }
  }
}