#include "guessing_game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Returns a random number with the current time as seed.
 *
 * @return int
 */
int GenerateRandomNumber(int N) {
  srand(time(NULL));  // Init seed to current time since UNIX 00:00:00 UTC
  int generated_number = rand() % (N + 1);
  return generated_number;
}

/**
 * @brief Read user guess from stdin.
 *
 * @return int
 */
int GetUserGuess() {
  int user_guess;
  scanf("%d", &user_guess);
  return user_guess;
}

/**
 * @brief Compares the user guess to the correct number and returns the
 * comparison.
 *
 * @param user_number
 * @param correct_number
 * @return true
 * @return false
 */
bool VerifiyGuess(int user_number, int correct_number) {
  return user_number == correct_number;
}