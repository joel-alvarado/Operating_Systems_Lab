#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
  printf("Thread is running\n");
  return NULL;
}

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, thread_function, NULL);  // Create a thread
  pthread_join(thread, NULL);  // Wait for the thread to finish
  return 0;
}
