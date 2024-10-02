#include <pthread.h>
#include <stdio.h>
#include <string.h>
typedef struct my_struct {
  int random_number;
  int* arr;
  int arr_size;
} my_struct;

void* thread_function(void* arg) {
  my_struct* m = (my_struct*)arg;
  for (int i = 0; i < m->arr_size; i++) {
    printf("%d ", m->arr[i]);
  }
  printf("Thread is running\n");
  printf("Tuabuela: %d\n", m->random_number);
  return NULL;
}

int main() {
  my_struct m;
  m.random_number = 32;
  int arr[3] = {1, 2, 3};

  int* arr_malloc = malloc(sizeof(int) * 3);
  memcpy(arr_malloc, &arr, sizeof(int) * 3);

  m.arr = arr_malloc;
  m.arr_size = 3;

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, thread_function,
                 &m);             // Create a thread
  pthread_join(thread_id, NULL);  // Wait for the thread to finish
  free(arr_malloc);
  return 0;
}
