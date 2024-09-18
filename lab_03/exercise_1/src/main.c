#include <math.h>
#include <pthread.h>
#include <stdio.h>

#include "asin_calc_threading.h"

int main(void) {
  pthread_t input_thread;   // First thread's ID
  pthread_t arcsin_thread;  // Second thread's ID

  t_user_locks locks;
  pthread_mutex_init(&locks.user_input_lock, NULL);
  pthread_mutex_init(&locks.exit_lock, NULL);
  pthread_cond_init(&locks.new_input_cond, NULL);
  pthread_cond_init(&locks.exit_cond, NULL);
  locks.exit = 0;

  pthread_create(&input_thread, NULL, user_input_thread_routine, &locks);
  pthread_create(&arcsin_thread, NULL, arcsin_calc_thread_routine, &locks);
  pthread_join(input_thread, NULL);
  pthread_join(arcsin_thread, NULL);

  pthread_mutex_destroy(&locks.user_input_lock);
  pthread_mutex_destroy(&locks.exit_lock);
  pthread_cond_destroy(&locks.new_input_cond);
  pthread_cond_destroy(&locks.exit_cond);
}