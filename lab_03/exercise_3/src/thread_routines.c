#include "thread_routines.h"

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  bool all_data_read;
  bool all_data_processed;
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
  FILE *file = fopen(thread_data->file_name, "r");

  if (file == NULL) {
    perror("fopen");
    pthread_exit((void *)EXIT_FAILURE);
  }

  for (int i = 0; i < thread_data->input_buffer_size; i++) {
    // Lock and read 1 value into input buffer
    pthread_mutex_lock(&thread_data->input_buffer_lock);
    fscanf(file, "%d", &thread_data->input_buffer[i]);
    pthread_mutex_unlock(&thread_data->input_buffer_lock);

    // Lock and update last read value
    pthread_mutex_lock(&thread_data->n_read_values_lock);
    thread_data->n_read_values++;
    pthread_cond_signal(&thread_data->start_sqrt_thread);
    pthread_mutex_unlock(&thread_data->n_read_values_lock);
  }
  pthread_mutex_lock(&thread_data->n_read_values_lock);
  thread_data->all_data_read = true;
  pthread_cond_signal(&thread_data->start_sqrt_thread);
  pthread_mutex_unlock(&thread_data->n_read_values_lock);
  pthread_exit(EXIT_SUCCESS);
}

void *calculateSquareRoot(void *data) {
  tThreadData *thread_data = (tThreadData *)data;

  while (1) {
    // Lock n_processed_values to get the latest number of processed values
    pthread_mutex_lock(&thread_data->n_processed_values_lock);
    int last_n_processed_values = thread_data->n_processed_values;
    pthread_mutex_unlock(&thread_data->n_processed_values_lock);

    // Wait for new data to be available
    pthread_mutex_lock(&thread_data->n_read_values_lock);
    while (thread_data->n_read_values == last_n_processed_values &&
           !thread_data->all_data_read) {
      pthread_cond_wait(&thread_data->start_sqrt_thread,
                        &thread_data->n_read_values_lock);
    }
    int last_n_read_values = thread_data->n_read_values;
    pthread_mutex_unlock(&thread_data->n_read_values_lock);

    // If there are new values to process
    if (last_n_read_values > last_n_processed_values) {
      // Lock the processed buffer and perform the square root
      pthread_mutex_lock(&thread_data->processed_buffer_lock);
      int i;
      for (i = last_n_processed_values; i < last_n_read_values; i++) {
        float r = thread_data->input_buffer[i];
        float y = r / 2;
        for (int j = 0; j < 10; j++) {
          y = 0.5 * (y + r / y);
        }
        thread_data->processed_buffer[i] = y;
      }
      pthread_mutex_unlock(&thread_data->processed_buffer_lock);

      // Lock n_processed_values and update the count of processed values
      pthread_mutex_lock(&thread_data->n_processed_values_lock);
      thread_data->n_processed_values = i;
      pthread_cond_signal(
          &thread_data->start_write_processed_buffer);  // Signal writer thread
      pthread_mutex_unlock(&thread_data->n_processed_values_lock);
    }

    // Exit if the last value has been processed
    pthread_mutex_lock(&thread_data->n_read_values_lock);
    if (thread_data->all_data_read &&
        last_n_processed_values >= thread_data->n_read_values) {
      pthread_mutex_unlock(&thread_data->n_read_values_lock);
      break;  // Break the loop after processing the last value
    }
    pthread_mutex_unlock(&thread_data->n_read_values_lock);
  }

  pthread_exit((void *)EXIT_SUCCESS);  // Gracefully exit the thread
}

void *writeProcessedBuffer(void *data) {
  tThreadData *thread_data = (tThreadData *)data;

  // MISSING CODE HERE
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
  thread_data.all_data_read = false;
  thread_data.all_data_processed = false;

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

  for (int i = 0; i < input_buffer_size; i++) {
    printf("%.2f\n", thread_data.processed_buffer[i]);
  }
}
