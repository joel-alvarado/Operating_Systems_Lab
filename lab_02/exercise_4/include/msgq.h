#ifndef MSGQ_H
#define MSGQ_H

#define MSG_ARRAY_SIZE 1000
#define SUM_SIZE 1000000

struct message {
  long mtype;
  int marray[MSG_ARRAY_SIZE];
};

struct time {
  long mtype;
  struct timeval start;
};

#endif /* MSGQ_H */