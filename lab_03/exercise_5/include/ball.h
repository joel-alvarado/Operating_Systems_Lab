#ifndef BALL_H
#define BALL_H

/**
 * @brief Struct to hold ball data
 *
 */
typedef struct {
  int x;
  int y;
} BallPosition;

/**
 * @brief Struct to hold shared thread data
 *
 */
typedef struct sThreadData {
  BallPosition ball_pos;
  int counter_val;
  int dx, dy;
  int running;
} tThreadData;

/**
 * @brief Thread routine that updates the balls' positions using dx, dy from
 * tThreadData
 *
 * @param arg Pointer to tThreadData struct for a specific ball
 * @return void*
 */
void* updateBallPosition(void* arg);

/**
 * @brief Thread routine that continuously draws the balls and counter on the
 * screen
 *
 * @param arg
 * @return void*
 */
void* drawScreen(void* arg);

/**
 * @brief Thread routine that increments and updates the counter value
 *
 * @param arg
 * @return void*
 */
void* updateCounter(void* arg);

/**
 * @brief Starts the main bouncing ball program. Creates threads for ball
 * movement, drawing, and counter updating.
 *
 * @return int Status code
 */
int startBouncingBall();

/**
 * @brief Creates a new ball with a random starting position, dx, and dy values
 *
 */
void createNewBall();

/**
 * @brief Changes the direction of all active balls by reversing their dx and dy
 * values
 *
 */
void changeBallDirection();

#endif
