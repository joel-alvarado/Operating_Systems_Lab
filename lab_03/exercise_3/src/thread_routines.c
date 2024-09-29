#include "thread_routines.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sThreadData {
  char *file_name;
  int *input_buffer;
  size_t input_buffer_size;
  int n_read_values;
  double *processed_buffer;
  size_t processed_buffer_size;
  int n_processed_values;

  pthread_mutex_t input_buffer_lock;
  pthread_mutex_t n_read_values_lock;
  pthread_mutex_t processed_buffer_lock;
  pthread_mutex_t n_processed_values_lock;

  pthread_cond_t start_sqrt_thread;
  pthread_cond_t start_write_processed_buffer;
} tThreadData;

void *writeRandomIntegers(void *data) {
  int thread_exit_status;
  tThreadData *thread_data = (tThreadData *)data;
  FILE *file = fopen(thread_data->file_name, "r");
  if (file == NULL) {
    file = fopen(thread_data->file_name, "w");

    srand(time(NULL));
    int written_numbers = 0;
    while (written_numbers < thread_data->input_buffer_size) {
      // Random unsigned 8 bit number [0, 255]
      __uint8_t randonm_number = rand() % (255 - 0 + 1) + 0;
      fprintf(file, "%d", randonm_number);
      if (written_numbers != thread_data->input_buffer_size - 1)
        fprintf(file, "\n");
      written_numbers++;
    }
    fclose(file);
    thread_exit_status = 0;
    printf("File '%s' created and random numbers written successfully.\n",
           thread_data->file_name);
  } else {
    thread_exit_status = 1;
    printf("Error: File '%s' already exists.\n", thread_data->file_name);
  }
  pthread_exit(&thread_exit_status);
}

void *readIntoInputBuffer(void *data) {
  tThreadData *thread_data = (tThreadData *)data;
}

void *calculateSquareRoot(void *data) {
  tThreadData *thread_data = (tThreadData *)data;
}

void *writeProcessedBuffer(void *data) {
  tThreadData *thread_data = (tThreadData *)data;
}

void startThreads() {
  // Init shared data between threads
  tThreadData thread_data;

  // File name stuff
  char *file_name = "data.txt";
  thread_data.file_name = file_name;

  // Input buffer stuff
  int input_buffer_size = 10000;
  int *input_buffer = (int *)malloc(input_buffer_size * sizeof(int));
  thread_data.input_buffer = input_buffer;
  thread_data.input_buffer_size = input_buffer_size;
  thread_data.n_read_values = 0;
  pthread_mutex_init(&thread_data.input_buffer_lock, NULL);
  pthread_mutex_init(&thread_data.n_read_values_lock, NULL);

  // Processed buffer stuff
  int processed_buffer_size = input_buffer_size;
  double *processed_buffer =
      (double *)malloc(processed_buffer_size * sizeof(double));
  thread_data.processed_buffer = processed_buffer;
  thread_data.processed_buffer_size = processed_buffer_size;
  thread_data.n_processed_values = 0;
  pthread_mutex_init(&thread_data.processed_buffer_lock, NULL);
  pthread_mutex_init(&thread_data.n_processed_values_lock, NULL);

  // Conditions to synchronize reading thread, processing thread and writing
  // output thread
  pthread_cond_init(&thread_data.start_sqrt_thread, NULL);
  pthread_cond_init(&thread_data.start_write_processed_buffer, NULL);

  // Launch writer thread
  pthread_t writer_thread_id;
  int *writer_thread_status;
  pthread_create(&writer_thread_id, NULL, writeRandomIntegers, &thread_data);
  pthread_join(writer_thread_id, (void **)writer_thread_status);

  // Launch reading thread, processing thread and writing output thread
  pthread_t read_into_input_buffer_thread_id;
  pthread_t calculate_square_root_thread_id;
  pthread_t write_processed_buffer_thread_id;

  pthread_create(&read_into_input_buffer_thread_id, NULL, readIntoInputBuffer,
                 &thread_data);
  pthread_create(&calculate_square_root_thread_id, NULL, calculateSquareRoot,
                 &thread_data);
  pthread_create(&write_processed_buffer_thread_id, NULL, writeProcessedBuffer,
                 &thread_data);

  pthread_join(read_into_input_buffer_thread_id, NULL);
  pthread_join(calculate_square_root_thread_id, NULL);
  pthread_join(write_processed_buffer_thread_id, NULL);
}
