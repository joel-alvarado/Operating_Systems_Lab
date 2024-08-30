#include <stdio.h>
#include <stdlib.h>

#include "text_handler.h"

int main() {
  printf("Welcome to Simple Text Handler.\n");
  printf("Max chars per line: %d\n", LINE_LENGTH_MAX);
  printf("Max lines: %d\n", LINE_AMOUNT_MAX);

  char **text = malloc(LINE_AMOUNT_MAX * sizeof(char *));
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    text[i] = malloc((LINE_LENGTH_MAX * sizeof(char)) + 2);  // +2 for \n & \0
  }

  GetUserInput(text);
  printf(
      "What would you like to do?\n1. Align Left\n2. Align Right\n3. "
      "Justify\n");
  int user_choice;
  scanf("%d", &user_choice);
  switch (user_choice) {
    case 1:
      AlignTextLeft(text);
      break;
    case 2:
      AlignTextRight(text);
      break;
    case 3:
      AlignTextJustify(text);
      break;
    default:
      break;
  }

  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    printf("%s\n", text[i]);
  }
}