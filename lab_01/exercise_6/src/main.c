#include <stdio.h>
#include <stdlib.h>

#include "text_handler.h"

int main() {
  printf("Welcome to simple text handler.\n");
  printf("Max chars per line: %d\n", LINE_LENGTH_MAX);
  printf("Max lines: %d\n", LINE_AMOUNT_MAX);

  char **text = malloc(LINE_AMOUNT_MAX * sizeof(char *));
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    text[i] = malloc(LINE_LENGTH_MAX * sizeof(char));
  }

  GetUserInput(text);
  AlignTextLeft(text);

  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    printf("%s", text[i]);
  }
}