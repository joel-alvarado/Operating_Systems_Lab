#ifndef ASIN_CALC_THREADING_H
#define ASIN_CALC_THREADING_H

#include <math.h>
#include <pthread.h>
#include <stdio.h>

/**
 * @brief Typedef struct to mantain mutex and cond for the thread routines
 * below.
 *
 */
typedef struct s_user_locks {
  pthread_mutex_t user_input_lock;
  pthread_mutex_t exit_lock;
  pthread_cond_t new_input_cond;
  pthread_cond_t exit_cond;
  int exit;
  float user_input;
} t_user_locks;

/**
 * @brief Thread routine that reads user input and signals a cond for arcsin
 * calculation.
 *
 * @param data
 * @return void*
 */
void *user_input_thread_routine(void *data);

/**
 * @brief If data->user_input is 0 >= x <= 1, the thread will calculate arcsin.
 * If x > 100, the thread will signal the input thread and itself to stop and
 * exit. Else, nothing will happen.
 *
 * @param data
 * @return void*
 */
void *arcsin_calc_thread_routine(void *data);

#endif /* ASIN_CALC_THREADING_H */