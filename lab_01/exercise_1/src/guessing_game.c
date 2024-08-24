#include "guessing_game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void GuessingGame(bool debug) {
  printf("Guess a number between 1 and %d: \n", MAX_GUESS_NUMBER);
  int correct_number = GenerateRandomNumber();

  if (debug) {
    printf("System chose: %d \n", correct_number);
  }

  while (true) {
    printf("Your guess: ");
    int user_number = GetUserGuess();
    if (VerifiyGuess(user_number, correct_number)) {
      printf("Correct! \n");
      return;
    } else {
      printf("Wrong! \n");
    }
  }
}

int GenerateRandomNumber() {
  srand(time(NULL));  // Init seed to current time since UNIX 00:00:00 UTC
  int generated_number = (rand() % MAX_GUESS_NUMBER + 1);
  return generated_number;
}

int GetUserGuess() {
  int user_guess;
  scanf("%d", &user_guess);
  return user_guess;
}

bool VerifiyGuess(int user_number, int correct_number) {
  return user_number == correct_number;
}