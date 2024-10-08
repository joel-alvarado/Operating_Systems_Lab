#include "asin_calc_threading.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>

typedef struct sUserLocks {
  pthread_mutex_t user_input_lock;
  pthread_mutex_t exit_lock;
  pthread_cond_t user_input_cond;
  pthread_cond_t exit_cond;
  int exit;
  float user_input;
} tUserLocks;

void *userInputThreadRoutine(void *data) {
  tUserLocks *locks = (tUserLocks *)data;

  while (1) {
    float input;
    scanf("%f", &input);

    // Send ssignal to asin calc with user input
    pthread_mutex_lock(&locks->user_input_lock);
    locks->user_input = input;
    pthread_cond_signal(&locks->user_input_cond);
    pthread_mutex_unlock(&locks->user_input_lock);

    // Wait for asin calc to signal with exit flag
    pthread_mutex_lock(&locks->exit_lock);
    pthread_cond_wait(&locks->exit_cond, &locks->exit_lock);
    int exit = locks->exit;
    pthread_mutex_unlock(&locks->exit_lock);

    if (exit) {
      break;
    }
  }
}

void *arcsinCalcThreadRoutine(void *data) {
  tUserLocks *locks = (tUserLocks *)data;
  while (1) {
    // Lock and wait for new user input
    pthread_mutex_lock(&locks->user_input_lock);
    pthread_cond_wait(&locks->user_input_cond, &locks->user_input_lock);
    float input = locks->user_input;
    pthread_mutex_unlock(&locks->user_input_lock);

    // Set exit flag if input > 100, signal to user_input thread the exit flag.
    pthread_mutex_lock(&locks->exit_lock);
    locks->exit = input > 100;
    pthread_cond_signal(&locks->exit_cond);
    pthread_mutex_unlock(&locks->exit_lock);

    // If 0 >= x <= 1, print arcsin
    if (input >= 0.0 && input <= 1.0) {
      float input_arcsin = asin(input);
      printf("Num: %.4f -> Arcsin: %.4f\n", input, input_arcsin);
    }

    // Finish thread
    if (input > 100) break;
  }
}

void launchAsinCalculator() {
  pthread_t input_thread;   // First thread's ID
  pthread_t arcsin_thread;  // Second thread's ID

  tUserLocks locks;
  pthread_mutex_init(&locks.user_input_lock, NULL);
  pthread_mutex_init(&locks.exit_lock, NULL);
  pthread_cond_init(&locks.user_input_cond, NULL);
  pthread_cond_init(&locks.exit_cond, NULL);
  locks.exit = 0;

  pthread_create(&input_thread, NULL, userInputThreadRoutine, &locks);
  pthread_create(&arcsin_thread, NULL, arcsinCalcThreadRoutine, &locks);
  pthread_join(input_thread, NULL);
  pthread_join(arcsin_thread, NULL);

  pthread_mutex_destroy(&locks.user_input_lock);
  pthread_mutex_destroy(&locks.exit_lock);
  pthread_cond_destroy(&locks.user_input_cond);
  pthread_cond_destroy(&locks.exit_cond);
}