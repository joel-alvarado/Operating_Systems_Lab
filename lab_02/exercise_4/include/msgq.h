#ifndef MSGQ_H
#define MSGQ_H

#define MSG_ARRAY_SIZE 1000
#define SUM_SIZE 1000000

/**
 * @brief Message struct to send using message queues
 *
 */
struct message {
  long mtype;
  int marray[MSG_ARRAY_SIZE];
};

/**
 * @brief Time struct to send timeval in message queues
 *
 */
struct time {
  long mtype;
  struct timeval start;
};

#endif /* MSGQ_H */