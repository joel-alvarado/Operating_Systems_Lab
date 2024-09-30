#ifndef THREAD_ROUTINES_H
#define THREAD_ROUTINES_H

#include <math.h>
#include <pthread.h>
#include <stdio.h>

#define FILE_NAME_LENGTH_MAX 128

/**
 * @brief Holds all shared data between threads
 *
 */
typedef struct sThreadData tThreadData;

/**
 * @brief Thread routine that writes `tThreadData.input_buffer_size` random
 * __uint8_t to `tThreadData.file_name`. Returns 0 if succesful and 1 if failed
 * (eg. file already exists)
 *
 * @param data
 * @return void*
 */
void *writeRandomIntegers(void *data);

/**
 * @brief Thread routine that reads `tThreadData.input_buffer_size` numbers into
 * `tThreadData.input_buffer` from `tThreadData.file_name`. Signals
 * `calculateSquareRoot` thread when new data is read into
 * `tThreadData.input_buffer`
 *
 * @param data
 * @return void*
 */
void *readIntoInputBuffer(void *data);

/**
 * @brief Thread routine that calculates the square root of
 * `tThreadData.input_buffer` (whose size is same as
 * `tThreadData.input_buffer_size`) and writes the result into
 * `tThreadData.processed_buffer`. Signals `writeProcessedBuffer` when newly
 * processed data is available. Also waits for `readIntoInputBuffer` for newly
 * read data.
 *
 * @param data
 * @return void*
 */
void *calculateSquareRoot(void *data);

/**
 * @brief Thread routine that reads from `tThreadData.processed_buffer` and
 * writes the numbers to `tThreadData.output_file_name`. This routines waits to
 * be signaled by `calculateSquareRoot` for newly processed numbers.
 *
 * @param data
 * @return void*
 */
void *writeProcessedBuffer(void *data);

/**
 * @brief Launches the main program. Will launch a thread with
 * `writeRandomIntegers`. Then launches 3 separate threads from
 * `readIntoInputBuffer`, `calculateSquareRoot` & `writeProcessedBuffer`. Waits
 * for these threads to be finished
 *
 */
void startThreads();

/**
 * @brief Helper function to calculate difference between 2 `timespec` structs
 *
 * @param start
 * @param end
 * @return double
 */
double calculateTimeDifference(struct timespec *start, struct timespec *end);

#endif /* THREAD_ROUTINES_H */