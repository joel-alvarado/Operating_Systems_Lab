#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Struct to hold ball data
 *
 */
typedef struct BallPosition;

/**
 * @brief Struct to hold shared thread data
 *
 */
typedef struct sThreadData tThreadData;

/**
 * @brief Thread routine that updates the balls position using dx, dy from
 * tThreadData
 *
 * @param arg
 * @return void*
 */
void* updateBall(void* arg);

/**
 * @brief
 * Thread routine that on screen counter using counter_val from tThreadData
 *
 * @param arg
 * @return void*
 */
void* updateCounter(void* arg);

/**
 * @brief Starts the main bouncing ball program. Uses updateBall and
 * updateCounter routines.
 *
 * @return int
 */
int startBouncingBall();