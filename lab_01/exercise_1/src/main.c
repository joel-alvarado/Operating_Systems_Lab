#include <stdio.h>

#include "guessing_game.h"

int main() {
  while (true) {
    int max_number = 9;
    printf("Guess a number between 0 and %d: ", max_number);
    int correct_number = GenerateRandomNumber(max_number);
    int user_number = GetUserGuess();
    printf("Correct %d \n", correct_number);

    if (VerifiyGuess(user_number, correct_number)) {
      printf("Correct! \n");
      return 0;
    } else {
      printf("Wrong! \n");
    }
  }
}