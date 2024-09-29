#include "tangent_calculator.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sThreadData {
  int assigned_section;
  int thread_count;
  double *input_array;
  double *result;
} tThreadData;

void *sumThreadRoutine(void *data) {
  tThreadData *thread_data = (tThreadData *)data;
  int assigned_section = thread_data->assigned_section;
  int assigned_section_size =
      TANGENT_INPUT_ARRAY_SIZE / thread_data->thread_count;

  double sum = 0;
  for (int i = 0; i < assigned_section_size; i++) {
    sum +=
        thread_data->input_array[assigned_section_size * assigned_section + i];
  }
  *thread_data->result = sum;
}

void fillTangentArray(double *array) {
  for (int i = 0; i < TANGENT_INPUT_ARRAY_SIZE; i++) {
    array[i] = tan(i + 1);
  }
}

double launchSumWithMultithreading(double *tangent_input_array) {
  int thread_count = 5;
  pthread_t thread_ids[thread_count];
  double results[thread_count];

  // Allocate memory for tCalculatorData for each thread
  tThreadData *data[thread_count];

  // Launch thread_count threads, each thread will have an assigned section
  // according to the order in which it is launched
  for (int i = 0; i < thread_count; i++) {
    data[i] = malloc(sizeof(tThreadData));  // Allocate each thread's data
    data[i]->thread_count = thread_count;
    data[i]->input_array = tangent_input_array;
    data[i]->assigned_section = i;
    data[i]->result = &results[i];
    pthread_create(&thread_ids[i], NULL, sumThreadRoutine, data[i]);
  }

  double total_from_threads = 0;
  for (int i = 0; i < thread_count; i++) {
    pthread_join(thread_ids[i], NULL);
    total_from_threads += results[i];
    free(data[i]);  // Free the allocated memory for each thread's data
  }
  return total_from_threads;
}

double launchSum(double *tangent_input_array) {
  double sum = 0;
  for (int i = 0; i < TANGENT_INPUT_ARRAY_SIZE; i++) {
    sum += tangent_input_array[i];
  }
  return sum;
}

void compareTimeTaken() {
  struct timeval start, end;
  double elapsed1, elapsed2;

  // Fill array from 1 to TANGENT_INPUT_ARRAY_SIZE
  double *tangent_input_array =
      (double *)malloc(TANGENT_INPUT_ARRAY_SIZE * sizeof(double));
  fillTangentArray(tangent_input_array);

  // Measure time for function1
  gettimeofday(&start, NULL);  // Start timing function 1
  double res1 = launchSumWithMultithreading(tangent_input_array);
  gettimeofday(&end, NULL);  // End timing function 1

  // Calculate time taken by function1
  elapsed1 = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
  printf("Total sum with multithreading: %.2f\n", res1);
  printf("Time taken by function 1: %.6f seconds\n", elapsed1);

  // Measure time for function2
  gettimeofday(&start, NULL);  // Start timing function 2
  double res2 = launchSum(tangent_input_array);
  gettimeofday(&end, NULL);  // End timing function 2

  // Calculate time taken by function2
  elapsed2 = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
  printf("Total sum with no multithreading: %.2f\n", res2);
  printf("Time taken by function 2: %.6f seconds\n", elapsed2);

  double diff = elapsed1 - elapsed2;
  printf("Improvement for multithreading vs single threding: %.6f seconds\n",
         diff);
}
