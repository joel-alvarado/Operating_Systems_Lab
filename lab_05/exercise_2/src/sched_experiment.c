#include "sched_experiment.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>  // for setpriority
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

void CPUBoundTask(int id) {
  printf("Process %d started, pid: %d\n", id, getpid());

  // Get start time
  struct timeval tv_start;
  gettimeofday(&tv_start, NULL);
  int count = 0;
  while (count < 30) {
    // CPU-bound work (infinite loop)
    for (volatile long i = 0; i < 1000000000; i++);
    // printf("Process %d still running, pid: %d\n", id, getpid());
    // printf("Process %d, counter %d\n", id, count);
    count++;
  }

  // Get time finished
  struct timeval tv_end;
  gettimeofday(&tv_end, NULL);
  double elapsed_time = (tv_end.tv_sec - tv_start.tv_sec) +
                        (tv_end.tv_usec - tv_start.tv_usec) / 1000000.0;

  printf("%d took %.6f seconds\n", id, elapsed_time);
}

int StartSchedulerExperiment(int priorities[3]) {
  pid_t pids[NUM_CHILDREN];

  // Create multiple CPU-bound processes with different priorities
  for (int i = 0; i < NUM_CHILDREN; i++) {
    pids[i] = fork();
    if (pids[i] < 0) {
      perror("fork");
      exit(1);
    }

    if (pids[i] == 0) {
      // In child process
      printf("Child %d (pid: %d) with priority %d\n", i + 1, getpid(),
             priorities[i]);

      // printf("The initial priority is %d\n",
      //        getpriority(PRIO_PROCESS, getpid()));

      // Set the priority of this process
      if (setpriority(PRIO_PROCESS, getpid(), priorities[i]) < 0) {
        perror("setpriority");
        exit(1);
      }

      // Start the CPU-bound task
      CPUBoundTask(i + 1);
      exit(0);  // Shouldn't reach here
    }
  }

  // Parent process: wait for all child processes
  for (int i = 0; i < NUM_CHILDREN; i++) {
    wait(NULL);  // Waiting for child processes (will block)
  }

  return 0;
}