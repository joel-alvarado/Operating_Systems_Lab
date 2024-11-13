#include <math.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

void TimerHandlerWithSignals(int x);

#define N_SAMPLES 1000
#define PERIOD_MS 10

int counter = 0;

int main(int argc, char *argv[]) {
  /// configura  el timer
  timer_t timer_id;
  timer_create(CLOCK_REALTIME, NULL, &timer_id);

  struct sigaction action;

  action.sa_handler = TimerHandlerWithSignals;
  action.sa_flags = 0;
  sigaction(SIGALRM, &action, NULL);

  /// Se crea el timer

  /*Se configuran los parámetros del timer*/
  struct itimerspec timerParams;
  /*Primer disparo*/
  timerParams.it_value.tv_sec = 0;
  timerParams.it_value.tv_nsec = PERIOD_MS * 1000000;
  /*Como es diferente de cero el timer es periódico*/
  timerParams.it_interval.tv_sec = 0;
  timerParams.it_interval.tv_nsec = PERIOD_MS * 1000000;

  /*Se arranca el timer (tiempo relativo)*/
  timer_settime(timer_id, 0, &timerParams, NULL);

  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_REALTIME, &start);

  while (counter < N_SAMPLES) {
    usleep(10000);
    // printf("%d\n", counter);
  }

  clock_gettime(CLOCK_REALTIME, &end);

  double elapsed =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

  printf("Elapsed time: %.9f seconds. Counter = %d \n", elapsed, counter);

  return (0);
}

void TimerHandlerWithSignals(int x) {
  counter++;
  char buf[64];
  snprintf(buf, sizeof(buf), "%d\n", counter);

  write(1, buf, strlen(buf));
}

// void TimerHandlerWithSignals(int x) {
//   static struct timespec prev_chrono_timer;

//   if (counter == 0) {
//     clock_gettime(CLOCK_REALTIME, &prev_chrono_timer);
//   }

//   struct timespec current_chrono_timer;
//   clock_gettime(CLOCK_REALTIME, &current_chrono_timer);

//   double elapsed_seconds_timer =
//       (current_chrono_timer.tv_sec - prev_chrono_timer.tv_sec) +
//       (current_chrono_timer.tv_nsec - prev_chrono_timer.tv_nsec) / 1E9;

//   char buf[64];
//   snprintf(buf, sizeof(buf), "%d %f\n", counter, elapsed_seconds_timer);

//   write(1, buf, strlen(buf));

//   // if (counter >= N_SAMPLES) {
//   //   exit(1);
//   // }
//   prev_chrono_timer = current_chrono_timer;
//   counter++;
// }
