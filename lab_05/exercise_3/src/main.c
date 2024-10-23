#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

void* thread_func(void* arg) {
  // Time execution of thread
  struct timeval tv_start;
  gettimeofday(&tv_start, NULL);  // Get current time

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

  int i = 0;
  while (i < 200) {
    // usleep(500000);
    for (volatile long j = 0; j < 1000000000; j++);
    printf("Thread %d running. iteration %d\n", num, i);
    i++;
  }

  // Get final time of execution
  struct timeval tv_end;
  gettimeofday(&tv_end, NULL);  // Get current time
  printf("Seconds: %ld\nMicroseconds: %ld\n", tv_end.tv_sec - tv_start.tv_sec,
         tv_end.tv_usec - tv_start.tv_usec);
}

int main() {
  pthread_t t1, t2, t3, t4;
  int arg1 = 1, arg2 = 2, arg3 = 3, arg4 = 4;

  struct sched_param param;
  pthread_attr_t attr;

  // Thread 1
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
  param.sched_priority = 50;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&t1, &attr, thread_func, &arg1);

  // Thread 2
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
  param.sched_priority = 50;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&t2, &attr, thread_func, &arg2);

  // Thread 3
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_RR);
  pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
  param.sched_priority = 50;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&t3, &attr, thread_func, &arg3);

  // Thread 4
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_RR);
  pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
  param.sched_priority = 50;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&t4, &attr, thread_func, &arg4);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);
  return 0;
}