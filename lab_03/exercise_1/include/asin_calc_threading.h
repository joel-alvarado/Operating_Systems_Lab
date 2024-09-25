#ifndef ASIN_CALC_THREADING_H
#define ASIN_CALC_THREADING_H

#include <math.h>
#include <pthread.h>
#include <stdio.h>

/**
 * @brief Typedef struct to mantain mutex and cond for the thread routines
 * below.
 *
 */
typedef struct sUserLocks tUserLocks;

/**
 * @brief Thread routine that reads user input and signals a cond for arcsin
 * calculation.
 *
 * @param data
 * @return void*
 */
void *userInputThreadRoutine(void *data);

/**
 * @brief If data->user_input is 0 >= x <= 1, the thread will calculate arcsin.
 * If x > 100, the thread will signal the input thread and itself to stop and
 * exit. Else, nothing will happen.
 *
 * @param data
 * @return void*
 */
void *arcsinCalcThreadRoutine(void *data);

/**
 * @brief Launches the main arcsin calculator program. Will launch threads
 * asking for user input & calculating the arcsin of the user's number.
 * If input > 100 the program exits.
 *
 */
void launchAsinCalculator();

#endif /* ASIN_CALC_THREADING_H */