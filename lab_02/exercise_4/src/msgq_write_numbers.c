#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>

#include "msgq.h"

int main() {
  key_t key;
  int msgid;
  struct message msg;
  struct time t;

  // Register unique key for message queue
  key = ftok("msgq_write_numbers", 24);
  if (key == -1) {
    perror("ftok error");
    exit(EXIT_FAILURE);
  }

  // Read or create message queue
  msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
  }

  // Send start time to queue
  t.mtype = 2;
  struct timeval start;
  gettimeofday(&start, NULL);
  t.start = start;
  if (msgsnd(msgid, &t, sizeof(t.start), 0) == -1) {
    perror("msgsnd failed");
    exit(EXIT_FAILURE);
  }

  // Initalize message data
  int local_numbers[SUM_SIZE];
  msg.mtype = 1;
  for (int i = 0; i < SUM_SIZE; i++) {
    local_numbers[i] = i + 1;
  }

  // Send the message to msgq_sum_numbers
  for (int i = 0; i < SUM_SIZE; i += 1000) {
    // Send 1000 elements at a time
    memcpy(msg.marray, &local_numbers[i], 1000 * sizeof(int));
    if (msgsnd(msgid, &msg, sizeof(msg.marray), 0) == -1) {
      perror("msgsnd failed");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
