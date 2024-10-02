#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int shared_data = 0;

void* modify_shared_data(void* arg) {
  pthread_mutex_lock(&lock);  // Lock
  shared_data++;              // Critical section
  printf("Shared data: %d\n", shared_data);
  pthread_mutex_unlock(&lock);  // Unlock
  return NULL;
}

int main() {
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, modify_shared_data, NULL);
  pthread_create(&thread2, NULL, modify_shared_data, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  return 0;
}
