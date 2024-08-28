#ifndef TIME_H
#define TIME_H

/**
 * @brief Represents time as hours, minutes & seconds.
 *
 */
typedef struct {
  int hours;
  int minutes;
  int seconds;
} Time;

/**
 * @brief Initializes and returns a Time struct.
 *
 * @param hours
 * @param minutes
 * @param seconds
 * @return Time*
 */
Time TimeInit(int hours, int minutes, int seconds);

/**
 * @brief Initalizes a Time struct with current system time.
 *
 * @return Time struct representation of current system time.
 */
Time TimeInitSystemTime();

/**
 * @brief Prints Time struct t in format "00:00:00".
 *
 * @param t: Time struct
 */
void PrintTime(Time t);

/**
 * @brief Read hours, minutes & seconds from user.
 *
 * @return Time struct with user hours, minutes & seconds.
 */
Time GetTime();

#endif  // TIME_H