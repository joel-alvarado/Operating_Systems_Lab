#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#define SIZE 1000000

int main() {
  const char *shm_name = "/shm";
  const char *sem_name = "/sem";
  int shm_size = SIZE * sizeof(int) + 1 * sizeof(int);

  // Open shared memory object
  int shm_fd = shm_open(shm_name, O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    exit(EXIT_FAILURE);
  }

  // Map shared memory
  int *shared_array =
      mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shared_array == MAP_FAILED) {
    perror("mmap failed");
    exit(EXIT_FAILURE);
  }

  // Open semaphore
  sem_t *sem = sem_open(sem_name, 0);
  if (sem == SEM_FAILED) {
    perror("sem_open failed");
    exit(EXIT_FAILURE);
  }

  struct timeval start;
  gettimeofday(&start, NULL);  // Start the timer
  shared_array[SIZE] = start.tv_usec;

  // Write numbers to shared memory
  for (int i = 0; i < SIZE; i++) {
    shared_array[i] = i + 1;
  }

  // Signal sum_numbers that the writing is done
  sem_post(sem);

  // Clean up resources
  munmap(shared_array, shm_size);
  sem_close(sem);

  return 0;
}
