#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* wait_function(void* arg) {
  pthread_mutex_lock(&lock);
  while (!ready) {  // Wait until ready is true
    pthread_cond_wait(&cond, &lock);
  }
  printf("Condition met, proceeding...\n");
  pthread_mutex_unlock(&lock);
  return NULL;
}

void* signal_function(void* arg) {
  pthread_mutex_lock(&lock);
  ready = 1;
  pthread_cond_signal(&cond);  // Signal the waiting thread
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, wait_function, NULL);
  pthread_create(&thread2, NULL, signal_function, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  return 0;
}
