#include "text_handler.h"

#include <stdio.h>
#include <string.h>

void GetUserInput(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    printf("Input line: ");
    char *line = buf[i];
    fgets(line, LINE_LENGTH_MAX, stdin);
  }
}

void AlignTextLeft(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    char *current_line = buf[i];
    char padded_line[strlen(current_line) + 1];
    strcpy(&padded_line, &current_line);

    // DISGUSTING CODE DOWN HERE
    // int padding_width = LINE_LENGTH_MAX - strlen(current_line);
    // char padding[padding_width + 1];
    // for (int j = 0; j < padding_width; j++) {
    //   padding[j] = " ";
    // }
    // strcat(&padded_line, &padding);
    // for (int j = 0; j < strlen(padded_line); j++) {
    //   if (padded_line[j] == '\n') {
    //     // padded_line[j] =
    //   }
    // }
    // buf[i] = &padded_line;
  }
}

void AlignTextRight(char **buf) {}