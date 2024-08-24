#include <stdbool.h>

#ifndef GUESSING_GAME_H
#define GUESSING_GAME_H

#define MAX_GUESS_NUMBER 10

/**
 * @brief Starts the guessing game. Will end when user guesses the number
 * correctly.
 *
 * @param debug: If true, will print the system guess to the user.
 *
 */
void GuessingGame(bool debug);

/**
 * @brief Generates a random number between 0 and MAX_GUESS_NUMBER
 *
 * @return int
 */
int GenerateRandomNumber();

/**
 * @brief Reads user guess from stdin.
 *
 * @return int
 */
int GetUserGuess();

/**
 * @brief Compares the user guess with the game's number.
 *
 * @param user_number
 * @param correct_number
 * @return true
 * @return false
 */
bool VerifiyGuess(int user_number, int correct_number);

#endif /* GUESSING_GAME_H */