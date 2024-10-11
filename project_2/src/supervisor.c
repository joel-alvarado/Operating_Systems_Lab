#include "supervisor.h"

#define _GNU_SOURCE

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

typedef struct sigaction sigaction_t;
typedef struct sigevent sigevent_t;
typedef struct itimerspec itimerspec_t;

pid_t latest_imu_driver_pid;

void StartIMUSupervisor() {
  // Set up signal handler
  sigaction_t sig_action;
  sig_action.sa_flags = SA_SIGINFO;
  sig_action.sa_handler = CheckAndRestartIMUDriver;
  sigemptyset(&sig_action.sa_mask);
  if (sigaction(SIGUSR1, &sig_action, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Create the timer
  sigevent_t sig_event;
  timer_t timer_id;
  sig_event.sigev_notify = SIGEV_SIGNAL;
  sig_event.sigev_signo = SIGUSR1;
  sig_event.sigev_value.sival_ptr = &timer_id;
  if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1) {
    perror("timer_create");
    exit(1);
  }

  itimerspec_t itimer_spec;
  itimer_spec.it_value.tv_sec = 1.5;
  itimer_spec.it_value.tv_nsec = 0;
  itimer_spec.it_interval.tv_sec = 1.5;
  itimer_spec.it_interval.tv_nsec = 0;

  if (timer_settime(timer_id, 0, &itimer_spec, NULL) == -1) {
    perror("timer_settime");
    exit(1);
  }

  while (1) {
    waitpid(latest_imu_driver_pid, NULL, 0);
    usleep(100 * 1000);
  }
}

pid_t GetPidByName(const char *process_name) {
  char command[256];
  snprintf(command, sizeof(command), "pgrep -x %s", process_name);
  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    perror("popen");
    return -1;
  }
  int pid = -1;
  if (fscanf(fp, "%d", &pid) == 1) {
    // Successfully retrieved a PID
  }
  pclose(fp);
  return pid;
}

void CheckAndRestartIMUDriver(int signum) {
  char *imu_process_name = "imu_driver";
  pid_t imu_process_pid = GetPidByName(imu_process_name);

  if (imu_process_pid == -1) {
    latest_imu_driver_pid = StartNewIMUDriverProcess();
  }
}

pid_t StartNewIMUDriverProcess() {
  pid_t pid = fork();
  if (pid == 0) {
    char *args[] = {"./imu_driver", NULL};
    execvp(args[0], args);
  }
  return pid;
}