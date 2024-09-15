#include "counter_handler.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

void print_pids() {
  printf("PIDs: [");
  for (int i = 0; i < process_count; i++) {
    printf("%d", local_array[i]);
    if (i != process_count - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

int read_pid() {
  int pid;
  int valid = 0;
  do {
    printf("PID: ");
    scanf("%d", &pid);
    for (int i = 0; i < process_count; i++) {
      if (local_array[i] == pid) {
        valid = 1;
        break;
      }
    }
    if (!valid) {
      printf("Invalid PID. Please enter a valid one.\n");
    }
  } while (!valid);
  return pid;
}

int main(int argc, char **argv) {
  // Get process count of shared memory object
  process_count = atoi(argv[1]);
  killed = 0;
  char *name = "shm";

  // Open the shared memory
  int shm_fd = shm_open(name, O_RDONLY, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    return 1;
  }

  // Map shared memory to process
  int *shared_array =
      mmap(0, process_count * sizeof(int), PROT_READ, MAP_SHARED, shm_fd, 0);
  if (shared_array == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }

  // Copy shared memory to local array
  local_array = malloc(process_count * sizeof(int));
  memcpy(local_array, shared_array, process_count * sizeof(int));

  while (killed != process_count) {
    printf("What would you like to do?\n");
    printf("1. Stop counter\n");
    printf("2. Continue counter\n");
    printf("3. Reverse counter\n");
    printf("4. Reset counter\n");
    printf("5. Kill counter\n");
    printf("6. Exit\n");
    print_pids(shared_array, process_count);

    int user_choice;
    scanf("%d", &user_choice);

    int pid;
    switch (user_choice) {
      case 1:
        pid = read_pid();
        kill(pid, SIGSTOP);
        break;
      case 2:
        pid = read_pid();
        kill(pid, SIGCONT);
        break;
      case 3:
        pid = read_pid();
        kill(pid, SIGUSR2);
        break;
      case 4:
        pid = read_pid();
        kill(pid, SIGUSR1);
        break;
      case 5:
        pid = read_pid();
        kill(pid, SIGTERM);
        for (int i = 0; i < process_count; i++) {
          if (shared_array[i] == pid) {
            local_array[i] = -1;
            killed++;
            break;
          }
        }
        break;
      case 6:
        exit(0);
        break;
      default:
        break;
    }
  }

  printf("All counters have been killed, stopping...\n");
  munmap(shared_array, process_count * sizeof(int));
  free(local_array);
  return 0;
}
