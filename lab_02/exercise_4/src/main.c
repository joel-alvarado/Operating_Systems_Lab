#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

int main() {
  printf("UNIX Message Queues vs Shared Memory Demo\n");

  printf("Shared Memory:\n");
  int pid_adder_shm = fork();
  if (pid_adder_shm == 0) {
    char* args[] = {"./shm_sum_numbers", NULL};
    if (execvp(args[0], args) == -1) {
      perror("Main execvp");
      exit(0);
    }
  }
  waitpid(pid_adder_shm, NULL, 0);

  printf("Message Queues:\n");
  int pid_adder_msgq = fork();
  if (pid_adder_msgq == 0) {
    char* args[] = {"./msgq_sum_numbers", NULL};
    if (execvp(args[0], args) == -1) {
      perror("Main execvp");
      exit(0);
    }
  }
  waitpid(pid_adder_msgq, NULL, 0);
}