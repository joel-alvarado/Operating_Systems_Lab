#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

void* thread_func(void* arg) {
  int num = *(int*)arg;

  int policy;
  struct sched_param param;
  pthread_getschedparam(pthread_self(), &policy, &param);

  const char* policy_str = (policy == SCHED_FIFO)    ? "SCHED_FIFO"
                           : (policy == SCHED_RR)    ? "SCHED_RR"
                           : (policy == SCHED_OTHER) ? "SCHED_OTHER"
                                                     : "UNKNOWN";

  printf("Thread %lu is running with policy %s and priority %d\n",
         pthread_self(), policy_str, param.sched_priority);

  // Time execution of thread
  struct timeval tv_start;
  gettimeofday(&tv_start, NULL);  // Get current time

  int i = 0;
  while (i < 200) {
    // usleep(500000);
    for (volatile long j = 0; j < 1000000000; j++);
    // printf("Thread %d running. iteration %d\n", num, i);
    i++;
  }

  // Get final time of execution
  struct timeval tv_end;
  gettimeofday(&tv_end, NULL);  // Get current time

  long seconds = tv_end.tv_sec - tv_start.tv_sec;
  long microseconds = tv_end.tv_usec - tv_start.tv_usec;
  if (microseconds < 0) {
    seconds -= 1;
    microseconds += 1000000;
  }

  printf("Thread %lu finished in %ld seconds and %ld microseconds.\n",
         pthread_self(), seconds, microseconds);
}

int main() {
  // Fix: set rlimit to unlimited for this process
  // NOTE: For this to work, it MUST be run as privileged user
  struct rlimit rlim;
  getrlimit(RLIMIT_RTPRIO, &rlim);
  rlim.rlim_cur = RLIM_INFINITY;
  setrlimit(RLIMIT_RTPRIO, &rlim);

  int baseline[4] = {50, 50, 50, 50};
  int e1[4] = {0, 0, 99, 99};
  int e2[4] = {99, 99, 0, 0};
  int e3[4] = {90, 10, 90, 10};
  int e4[4] = {20, 20, 80, 80};
  int e5[4] = {80, 80, 20, 20};

  int* arr[] = {baseline, e1, e2, e3, e4, e5};

  for (int i = 0; i < 4; i++) {
    pthread_t t1, t2, t3, t4;
    int arg1 = 1, arg2 = 2, arg3 = 3, arg4 = 4;

    struct sched_param param;
    pthread_attr_t attr;

    // Thread 1
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    param.sched_priority = arr[i][0];
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t1, &attr, thread_func, &arg1);

    // Thread 2
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    param.sched_priority = arr[i][1];
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t2, &attr, thread_func, &arg2);

    // Thread 3
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    param.sched_priority = arr[i][2];
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t3, &attr, thread_func, &arg3);

    // Thread 4
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    param.sched_priority = arr[i][3];
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&t4, &attr, thread_func, &arg4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
  }
  return 0;
}