#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

int *plotting_block; // Stores global mmapped array

void *ReadData(void *args) {
  printf("ReadData run\n");
  while(1) {
    // Read input from user
    int input;
    scanf("%d", &input);
    
    // Fill array with range [input, input + 1000]
    int i = 0;
    while(i < 1000) {
      plotting_block[i] = input;
      input++;
      i++;
    }
  }
}

void PrintData(int signal) {
  // Print numbers currently stored in plotting_block
  printf("Timer fired\n");
  printf("[");
  for(int i = 0; i < 20; i++) {
    printf("%d ", plotting_block[i]);
  }
  printf("]\n");
}

int main() {
  // Set current process priority to lowest nice-ness value
  setpriority(PRIO_PROCESS, 0, -20);

  // Allocate space for 1000 integers from mapped segment 0xBEEDAD of memory
  plotting_block = mmap(0xBEEDAD, 1000 * sizeof(int), PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  // Create timer to fire SIGUSR2
  timer_t timer_id;
  struct sigevent sigev;
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = SIGUSR2;
  timer_create(CLOCK_REALTIME, &sigev, &timer_id);

  // Set up signal handler for SIGUSR2
  struct sigaction action;
  action.sa_handler = PrintData;
  action.sa_flags = 0;
  sigaction(SIGUSR2, &action, NULL);

  // Set up thread (will run infinitely)
  pthread_t tid;
  pthread_create(&tid, NULL, ReadData, NULL);

  // Start timer (first fire after 1.9s, consecutive fires every 2.2s)
  struct itimerspec timer_params;
  timer_params.it_value.tv_sec = 1;
  timer_params.it_value.tv_nsec = 900000000;
  timer_params.it_interval.tv_sec = 2;
  timer_params.it_interval.tv_nsec = 200000000;
  timer_settime(timer_id, 0, &timer_params, NULL);

  pthread_join(tid, NULL); // Wait for ReadData thread to die (basically never)
  return 0;
}