#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

void cpu_bound_task(int id) {
    printf("Process %d started, pid: %d\n", id, getpid());
    while (1) {
        // CPU-bound work (infinite loop)
        for (volatile long i = 0; i < 1000000000; i++);
        printf("Process %d still running, pid: %d\n", id, getpid());
    }
}

int main() {
    pid_t pids[NUM_CHILDREN];

    // Create multiple CPU-bound processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }

        if (pids[i] == 0) {
            // In child process
            cpu_bound_task(i + 1);
            exit(0); // Shouldn't reach here
        }
    }

    // Parent process: wait for all child processes (optional)
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL); // Waiting for child processes (will block)
    }

    return 0;
}