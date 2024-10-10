#include "driver.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct sMotionSensorInfo {
  double time_elapsed;
  double p_x;
  double p_y;
  double v_x;
  double v_y;
  double acc_x;
  double acc_y;
} MotionSensorInfo;

void StartIMUDriver() {
  MotionSensorInfo *info;
  char *fname = "data.txt";
  if (access(fname, F_OK) == 0) {
    printf("'data.txt' found. Loading latest sensor data...\n");
    info = ReadLatestSensorData();
  } else {
    printf("'data.txt' not found. Loading default sensor data...\n");
    info = malloc(sizeof(MotionSensorInfo));
    // Default sensor values
    info->time_elapsed = 0;
    info->p_x = 20;
    info->p_y = 10;
    info->v_x = 0;
    info->v_y = 0;
    info->acc_x = 0.15;
    info->acc_y = 0.06;
  }

  // Launch writer thread
  pthread_t writer_thread_id;
  void *writer_thread_exit;
  pthread_create(&writer_thread_id, NULL, WriteDataRoutine, info);
  pthread_join(writer_thread_id, writer_thread_exit);
}

void *WriteDataRoutine(void *sensor_info) {
  MotionSensorInfo *info = (MotionSensorInfo *)sensor_info;

  // Bandaid: Prevent duplicate info from being printed
  if (info->time_elapsed > 0) {
    UpdateSensorInfo(info);
  }

  while (1) {
    FILE *f = fopen("data.txt", "a");
    fprintf(f, "%.2f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", info->time_elapsed,
            info->p_x, info->p_y, info->v_x, info->v_y, info->acc_x,
            info->acc_y);
    fclose(f);

    int kMillisecondsToMicroseconds = 1000;
    usleep(200 * kMillisecondsToMicroseconds);
    UpdateSensorInfo(info);
  }
}

void UpdateSensorInfo(MotionSensorInfo *sensor_info) {
  // Increase time ellapsed by 200ms
  sensor_info->time_elapsed += 0.2;

  // Calculate new velocity with current acceleration
  sensor_info->v_x =
      sensor_info->v_x + sensor_info->acc_x * sensor_info->time_elapsed;
  sensor_info->v_y =
      sensor_info->v_y + sensor_info->acc_y * sensor_info->time_elapsed;

  // Calculate new position with new velocity
  sensor_info->p_x =
      sensor_info->p_x + sensor_info->v_x * sensor_info->time_elapsed;
  sensor_info->p_y =
      sensor_info->p_y + sensor_info->v_y * sensor_info->time_elapsed;
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
  info->time_elapsed = values_read[0];
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