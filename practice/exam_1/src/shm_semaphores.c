#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

// Semaphore operation structs
struct sembuf sem_lock = {0, -1, 0};   // P (wait) operation
struct sembuf sem_unlock = {0, 1, 0};  // V (signal) operation

int main() {
  key_t key;
  int shmid, semid;
  char *shared_memory;

  // Generate a unique key
  key = ftok("shmfile", 65);

  // Create shared memory segment
  shmid = shmget(key, 1024, 0666 | IPC_CREAT);
  shared_memory = (char *)shmat(shmid, NULL, 0);  // Attach to shared memory

  // Create a semaphore set with one semaphore
  semid = semget(key, 1, 0666 | IPC_CREAT);

  // Initialize the semaphore to 1 (available)
  semctl(semid, 0, SETVAL, 1);

  // Example 1: Writing to shared memory (process A)
  if (fork() == 0) {  // Child process (Process A)
    printf("Process A waiting to access shared memory...\n");
    semop(semid, &sem_lock, 1);  // Lock the semaphore (P operation)
    printf("Process A writing to shared memory...\n");
    strcpy(shared_memory, "Hello from Process A");
    sleep(2);  // Simulate some work
    printf("Process A done writing, unlocking semaphore.\n");
    semop(semid, &sem_unlock, 1);  // Unlock the semaphore (V operation)
    shmdt(shared_memory);          // Detach from shared memory
  } else {                         // Parent process (Process B)
    sleep(1);                      // Let Process A start first

    printf("Process B waiting to access shared memory...\n");
    semop(semid, &sem_lock, 1);  // Lock the semaphore (P operation)
    printf("Process B reading from shared memory: %s\n", shared_memory);
    printf("Process B done reading, unlocking semaphore.\n");
    semop(semid, &sem_unlock, 1);  // Unlock the semaphore (V operation)
    shmdt(shared_memory);          // Detach from shared memory

    // Cleanup: remove shared memory and semaphore
    shmctl(shmid, IPC_RMID, NULL);  // Destroy the shared memory
    semctl(semid, 0, IPC_RMID);     // Remove the semaphore
  }

  return 0;
}
