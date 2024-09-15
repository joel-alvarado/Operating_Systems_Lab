#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "msgq.h"

int main() {
  key_t key;
  int msgid;
  struct message msg;

  // Generate unique key for the message queue
  key = ftok("msgq_write_numbers", 24);
  if (key == -1) {
    perror("ftok error");
    exit(EXIT_FAILURE);
  }

  // Create or get the message queue
  msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
  }

  int pid_writer = fork();
  if (pid_writer == -1) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }

  if (pid_writer == 0) {
    char *args[] = {"./msgq_write_numbers", NULL};
    if (execvp(args[0], args) == -1) {
      perror("execvp failed");
      exit(EXIT_FAILURE);
    }
  }

  struct time t;
  if (msgrcv(msgid, &t, sizeof(t.start), 2, 0) == -1) {
    perror("msgrcv failed");
    exit(EXIT_FAILURE);
  }

  long total = 0;
  int local_numbers[SUM_SIZE];
  for (int i = 0; i < SUM_SIZE; i += 1000) {
    // Send 1000 elements at a time
    if (msgrcv(msgid, &msg, sizeof(msg.marray), 1, 0) == -1) {
      perror("msgrcv failed");
      exit(EXIT_FAILURE);
    }
    memcpy(&local_numbers[i], msg.marray, 1000 * sizeof(int));
  }
  for (int i = 0; i < SUM_SIZE; i++) {
    total += local_numbers[i];
  }

  // Finish timing
  struct timeval end;
  gettimeofday(&end, NULL);
  long seconds = end.tv_sec - t.start.tv_sec;
  long microseconds = end.tv_usec - t.start.tv_usec;
  if (microseconds < 0) {
    seconds -= 1;
    microseconds += 1000000;  // Convert 1 second to microseconds
  }
  long total_time_usec = seconds * 1000000 + microseconds;
  printf("Sum of numbers in message queue: %ld\n", total);
  printf("Time taken: %ld microseconds\n", total_time_usec);
  return 0;
}
