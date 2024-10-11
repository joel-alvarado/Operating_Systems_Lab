#include "driver.h"

#define _GNU_SOURCE

#include <bits/types/timer_t.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct sigaction sigaction_t;
typedef struct sigevent sigevent_t;
typedef struct itimerspec itimerspec_t;

typedef struct sMotionSensorInfo {
  double time_elapsed;
  double p_x;
  double p_y;
  double v_x;
  double v_y;
  double acc_x;
  double acc_y;
} MotionSensorInfo;

struct timespec last_update_time;
MotionSensorInfo *info;
timer_t timer_id;

void StartIMUDriver() {
  signal(SIGTERM, HandleSIGTERM);
  clock_gettime(CLOCK_REALTIME, &last_update_time);

  // Makes sure if data exists, load that instead
  char *file_name = "data.txt";
  if (access(file_name, F_OK) == 0) {
    printf("'data.txt' found. Loading latest sensor data...\n");
    info = ReadLatestSensorData();
  } else {
    printf("'data.txt' not found. Loading default sensor data...\n");
    info = GetDefaultSensorData();
  }

  // Set up signal handler
  sigaction_t sig_action;
  sig_action.sa_flags = SA_SIGINFO;
  sig_action.sa_handler = SaveSensorData;  // Specify the function to call
  sigemptyset(&sig_action.sa_mask);
  if (sigaction(SIGALRM, &sig_action, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Create the timer
  sigevent_t sig_event;
  sig_event.sigev_notify = SIGEV_SIGNAL;
  sig_event.sigev_signo = SIGALRM;
  sig_event.sigev_value.sival_ptr = &timer_id;
  if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1) {
    perror("timer_create");
    exit(1);
  }

  // Configure the timer to expire after 200ms and repeat every 200ms
  itimerspec_t itimer_spec;
  itimer_spec.it_value.tv_sec = 0;
  itimer_spec.it_value.tv_nsec = 200 * 1000000;  // 200 milliseconds
  itimer_spec.it_interval.tv_sec = 0;
  itimer_spec.it_interval.tv_nsec = 200 * 1000000;  // 200 milliseconds

  if (timer_settime(timer_id, 0, &itimer_spec, NULL) == -1) {
    perror("timer_settime");
    exit(1);
  }

  // Let the main program run indefinitely to allow the timer to trigger
  while (1) {
    pause();  // Pause until sig is recieved
  }
}

void SaveSensorData(int signum) {
  printf("PID %d writing to data.txt...\n", getpid());
  UpdateSensorInfo();
  FILE *f = fopen("data.txt", "a");
  if (!f) {
    perror("Error opening file for writing");
    return;
  }

  fprintf(f, "%.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", info->time_elapsed,
          info->p_x, info->p_y, info->v_x, info->v_y, info->acc_x, info->acc_y);
  fflush(f);
  fclose(f);
}

void UpdateSensorInfo() {
  // Capture current time
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  // Calculate the time elapsed since the last update (delta time)
  double elapsed_since_last_update =
      (current_time.tv_sec - last_update_time.tv_sec) +
      (current_time.tv_nsec - last_update_time.tv_nsec) / 1e9;

  // Update the reference time for the next update
  last_update_time = current_time;

  // Accumulate the total elapsed time
  info->time_elapsed += elapsed_since_last_update;

  // Update velocity using the time delta (not the total elapsed time)
  info->v_x += info->acc_x * elapsed_since_last_update;
  info->v_y += info->acc_y * elapsed_since_last_update;

  // Update position using the velocity and the time delta
  info->p_x += info->v_x * elapsed_since_last_update;
  info->p_y += info->v_y * elapsed_since_last_update;
}

MotionSensorInfo *ReadLatestSensorData() {
  FILE *f = fopen("data.txt", "r");
  if (!f) {
    perror("Error opening file");
    exit(1);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *lastLine = NULL;

  // Read each line to find the last one
  while ((read = getline(&line, &len, f)) != -1) {
    free(lastLine);           // Free the previous lastLine
    lastLine = strdup(line);  // Store as last line
  }

  free(line);  // Free the line buffer
  fclose(f);

  // Now parse the last line
  float values_read[7];
  if (lastLine) {
    // Tokenize the last line and parse each value as float
    char *token = strtok(lastLine, ", ");
    int i = 0;
    while (token) {
      float value = strtof(token, NULL);
      values_read[i] = value;
      token = strtok(NULL, ", ");
      i++;
    }
    free(lastLine);  // Free the memory allocated for the last line
  }

  // Now load values read into sensor info
  MotionSensorInfo *info = malloc(sizeof(MotionSensorInfo));
  time_t last_total_elapsed =
      values_read[0];  // Read the previous total elapsed time from file
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  // Calculate the time elapsed in the current session (in seconds with decimal
  // precision)
  double elapsed_since_start =
      (current_time.tv_sec - last_update_time.tv_sec) +
      (current_time.tv_nsec - last_update_time.tv_nsec) / 1e9;

  info->time_elapsed = elapsed_since_start + values_read[0];
  info->p_x = values_read[1];
  info->p_y = values_read[2];
  info->v_x = values_read[3];
  info->v_y = values_read[4];
  info->acc_x = values_read[5];
  info->acc_y = values_read[6];
  return info;
}

MotionSensorInfo *GetDefaultSensorData() {
  MotionSensorInfo *info = malloc(sizeof(MotionSensorInfo));
  // Default sensor values
  info->time_elapsed = 0;
  info->p_x = 20;
  info->p_y = 10;
  info->v_x = 0;
  info->v_y = 0;
  info->acc_x = 0.15;
  info->acc_y = 0.06;
  return info;
}

void HandleSIGTERM(int signum) {
  timer_delete(timer_id);
  exit(1);
}