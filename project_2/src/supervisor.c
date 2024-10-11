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

// Typedefd to avoid having to write struct
typedef struct sigaction sigaction_t;
typedef struct sigevent sigevent_t;
typedef struct itimerspec itimerspec_t;

pid_t latest_supervised_pid;
char *supervised_process_name;

void StartSupervisor(char *process_name) {
  // Set up supervised process
  supervised_process_name = process_name;

  // Set up signal handler to check & restart process
  sigaction_t sig_action;
  sig_action.sa_flags = SA_SIGINFO;
  sig_action.sa_handler = CheckAndRestartProcess;
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
    waitpid(latest_supervised_pid, NULL, 0);
    usleep(100 * 1000);
  }
}

pid_t GetPidByName(const char *process_name) {
  char *parsed_processed_name = malloc(strlen(process_name) * sizeof(char) + 1);
  strcpy(parsed_processed_name, process_name);

  // Handle relative path
  if (process_name[0] == '.' && process_name[1] == '/') {
    memcpy(parsed_processed_name, &parsed_processed_name[2],
           strlen(process_name) - 2);
    parsed_processed_name[strlen(process_name) - 2] = '\0';
  }

  // Handle absolute path
  if (process_name[0] == '/') {
    memcpy(parsed_processed_name, &parsed_processed_name[1],
           strlen(process_name) - 1);
    parsed_processed_name[strlen(process_name) - 1] = '\0';
  }

  char command[256];
  snprintf(command, sizeof(command), "pgrep -x %s", parsed_processed_name);
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

void CheckAndRestartProcess(int signum) {
  pid_t process_pid = GetPidByName(supervised_process_name);

  if (process_pid == -1) {
    latest_supervised_pid = StartNewSupervisedProcess();
  }
}

pid_t StartNewSupervisedProcess() {
  pid_t pid = fork();
  if (pid == 0) {
    char *args[] = {supervised_process_name, NULL};
    execvp(args[0], args);
  }
  return pid;
}