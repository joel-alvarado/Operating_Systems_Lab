#include "spawn_counters.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

void handle_sigchild(int sign) { killed++; }

int main(int argc, char **argv) {
  signal(SIGCHLD, handle_sigchild);
  counter_amount = atoi(argv[1]);
  killed = 0;
  int counter_pids[counter_amount];
  for (int i = 0; i < counter_amount; i++) {
    int p = fork();
    if (p == -1) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
    if (p == 0) {
      char *args[] = {"./infinite_counter", NULL};
      if (execvp(args[0], args) == -1) {
        perror("execvp");
      }
      exit(EXIT_FAILURE);
    }
    counter_pids[i] = p;
  }

  char *name = "shm";
  // Create shared memory object
  int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    return 1;
  }

  // Set size of shared memory
  ftruncate(shm_fd, counter_amount * sizeof(int));

  // Map shared memory to process
  shared_array = mmap(0, counter_amount * sizeof(int), PROT_READ | PROT_WRITE,
                      MAP_SHARED, shm_fd, 0);
  if (shared_array == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }

  // Write array to shared memory
  memcpy(shared_array, counter_pids, counter_amount * sizeof(int));

  while (1) {
    if (killed == counter_amount) {
      exit(EXIT_SUCCESS);
    }
  }

  // Clean up
  munmap(shared_array, counter_amount * sizeof(int));
  shm_unlink(name);
  close(shm_fd);
};