#define _POSIX_C_SOURCE 199309L

#include "plotter.h"

#include <curses.h> /* ncurses.h includes stdio.h */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ENTER_NCURSES 10

Point2D **plotted_points;  // Array of pointers to Point2D, will eventually hold
                           // all points
int point_amount;
int last_loaded_point_idx = 0;
pthread_mutex_t points_mutex =
    PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread synchronization

void HandlerTimerExpire(int sig) {
  pthread_mutex_lock(
      &points_mutex);  // Lock mutex before modifying plotted_points
  if (last_loaded_point_idx < point_amount) {
    LoadNextDataPoint();
  }
  pthread_mutex_unlock(&points_mutex);  // Unlock the mutex
}

void *DrawRoutine(void *args) {
  while (1) {
    pthread_mutex_lock(
        &points_mutex);  // Lock the mutex before accessing plotted_points

    // Draw all points
    erase();
    for (int i = 0; i < last_loaded_point_idx; i++) {
      Point2D *current_point = plotted_points[i];
      if (current_point) {
        mvprintw(current_point->y, current_point->x, "*");
      }
    }
    refresh();

    pthread_mutex_unlock(&points_mutex);  // Unlock the mutex

    usleep(20000);
  }
  return NULL;
}

int StartMousePlotter() {
  // Allocate memory for all points captured in data
  point_amount = GetFileSize("mouse_data.dat") /
                 (2 * sizeof(int));  // Each point has 2 ints (x, y)
  plotted_points = calloc(
      point_amount, sizeof(Point2D *));  // Initialize all pointers to NULL

  if (!plotted_points) {
    fprintf(stderr, "Failed to allocate memory for plotted_points.\n");
    return -1;
  }

  // Set up the signal handler
  struct sigaction sa;
  sa.sa_handler = HandlerTimerExpire;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Create the timer
  timer_t timer_id;
  struct sigevent sig_event;
  sig_event.sigev_notify = SIGEV_SIGNAL;
  sig_event.sigev_signo = SIGALRM;
  sig_event.sigev_value.sival_ptr = &timer_id;
  if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1) {
    perror("timer_create");
    exit(1);
  }

  // Configure the timer to expire after 1ms and repeat every 10ms
  struct itimerspec itimer_spec;
  itimer_spec.it_value.tv_sec = 0;
  itimer_spec.it_value.tv_nsec = 1 * 1000000;  // Start immediately
  itimer_spec.it_interval.tv_sec = 0;
  itimer_spec.it_interval.tv_nsec = 10 * 1000000;  // 10 milliseconds
  if (timer_settime(timer_id, 0, &itimer_spec, NULL) == -1) {
    perror("timer_settime");
    exit(1);
  }

  // Initialize ncurses
  initscr();
  pthread_t draw_tid;
  pthread_create(&draw_tid, NULL, DrawRoutine, NULL);

  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  int key_stroke;
  int break_loop = 0;
  while (!break_loop) {
    key_stroke = getch();
    switch (key_stroke) {
      case ENTER_NCURSES:
        break_loop = 1;  // Exit the loop
        break;
    }
  }

  // Clean up
  endwin();
  for (int i = 0; i < last_loaded_point_idx; i++) {
    free(plotted_points[i]);
  }
  free(plotted_points);

  return 0;
}

int GetFileSize(const char *fname) {
  FILE *fp = fopen(fname, "rb");
  if (!fp) {
    fprintf(stderr, "Failed to open file: %s\n", fname);
    return 0;
  }
  fseek(fp, 0L, SEEK_END);
  long sz = ftell(fp);
  fclose(fp);
  return sz;
}

void LoadNextDataPoint() {
  FILE *mouse_data_file = fopen("mouse_data.dat", "rb");
  if (!mouse_data_file) {
    fprintf(stderr, "Failed to open mouse_data.dat for reading.\n");
    return;
  }

  if (last_loaded_point_idx < point_amount) {
    fseek(mouse_data_file, last_loaded_point_idx * 8,
          SEEK_SET);  // Seek next pair of coords

    int coords[2];
    if (fread(coords, sizeof(int), 2, mouse_data_file) == 2) {
      Point2D *p = malloc(sizeof(Point2D));
      if (!p) {
        fprintf(stderr, "Failed to allocate memory for Point2D.\n");
        fclose(mouse_data_file);
        return;
      }
      p->x = coords[0];
      p->y = coords[1];
      plotted_points[last_loaded_point_idx] = p;
      last_loaded_point_idx++;
    }
  }

  fclose(mouse_data_file);
}
