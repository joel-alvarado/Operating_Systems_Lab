#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <wait.h>

#define SIZE 1000000

int main() {
  const char *shm_name = "/shm";
  const char *sem_name = "/sem";
  int shm_size = SIZE * sizeof(int);

  // Create shared memory object
  int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    exit(EXIT_FAILURE);
  }

  // Resize the shared memory object
  ftruncate(shm_fd, shm_size);

  // Map the shared memory
  int *shared_array =
      mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shared_array == MAP_FAILED) {
    perror("mmap failed");
    exit(EXIT_FAILURE);
  }

  // Create a semaphore initialized to 0
  sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 0);
  if (sem == SEM_FAILED) {
    perror("sem_open failed");
    exit(EXIT_FAILURE);
  }

  int pid = fork();
  if (pid == -1) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    char *args[] = {"./shm_write_numbers", NULL};
    if (execvp(args[0], args) == -1) {
      perror("execvp failed");
      exit(EXIT_FAILURE);
    }
  }

  sem_wait(sem);  // Wait until writer signals completion

  // Sum the numbers in shared memory
  long total = 0;
  for (int i = 0; i < SIZE; i++) {
    total += shared_array[i];
  }

  // Finish timing
  struct timeval end;
  gettimeofday(&end, NULL);
  int start_usec = shared_array[SIZE];
  int useconds = end.tv_usec - start_usec;
  printf("Sum of numbers in shared memory: %ld\n", total);
  printf("Time taken: %d microseconds\n", useconds);

  // Clean up resources
  munmap(shared_array, shm_size);
  shm_unlink(shm_name);
  sem_close(sem);
  sem_unlink(sem_name);

  return 0;
}
