#ifndef THREAD_ROUTINES_H
#define THREAD_ROUTINES_H

#include <math.h>
#include <pthread.h>
#include <stdio.h>

#define FILE_NAME_LENGTH_MAX 128

typedef struct sThreadData tThreadData;

void *writeRandomIntegers(void *data);
void *readIntoInputBuffer(void *data);
void *calculateSquareRoot(void *data);
void *writeProcessedBuffer(void *data);
void startThreads();

#endif /* THREAD_ROUTINES_H */