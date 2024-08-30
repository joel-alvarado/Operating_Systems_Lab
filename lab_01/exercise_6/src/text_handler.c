#include "text_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

void GetUserInput(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    printf("Input line: ");
    char *line = buf[i];
    fgets(line, LINE_LENGTH_MAX + 2, stdin);
  }
}

void AlignTextLeft(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    char *current_line = buf[i];
    current_line[strlen(current_line) - 1] = '\0';  // Remove \n from end
    int current_line_width = strlen(current_line);
    int padding_width = LINE_LENGTH_MAX - current_line_width;
    char padded_line[current_line_width + padding_width + 1];

    // Create space padding
    char padding[padding_width + 1];
    strcpy(&padding, "");
    for (int j = 0; j < padding_width; j++) {
      strcat(&padding, " ");
    }

    // Append into padded line current line then padding (padding at the end)
    strcpy(&padded_line, current_line);
    strcat(&padded_line, &padding);
    strcpy(buf[i], &padded_line);
  }
}

void AlignTextRight(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    char *current_line = buf[i];
    current_line[strlen(current_line) - 1] = '\0';  // Remove \n from end
    int current_line_width = strlen(current_line);
    int padding_width = LINE_LENGTH_MAX - current_line_width;
    char padded_line[current_line_width + padding_width + 1];

    // Create space padding
    char padding[padding_width + 1];
    strcpy(&padding, "");
    for (int j = 0; j < padding_width; j++) {
      strcat(&padding, " ");
    }

    // Append into padded line padding then current line (padding at the start)
    strcpy(&padded_line, &padding);
    strcat(&padded_line, current_line);

    strcpy(buf[i], &padded_line);
  }
}

void AlignTextJustify(char **buf) {
  for (int i = 0; i < LINE_AMOUNT_MAX; i++) {
    char *current_line = buf[i];
    current_line[strlen(current_line) - 1] = '\0';  // Remove \n from end
    int current_line_width = strlen(current_line);
    int remaining_space = LINE_LENGTH_MAX - current_line_width;

    int j = 0;
    while (remaining_space > 0) {
      if (j >= strlen(current_line)) {
        j = 0;
      }
      // Add space when word ends
      if (isspace(current_line[j])) {
        InsertAt(current_line, ' ', j);
        remaining_space--;
        // Find next word to skip to
        while (j < strlen(current_line)) {
          if (isalnum(current_line[j])) {
            break;
          }
          j++;
        }
      }
      j++;
    }
  }
}

void InsertAt(char *dst, char src, int pos) {
  int dst_len = strlen(dst);
  if (pos > dst_len) {
    return;
  }
  memmove(dst + pos + 1, dst + pos, dst_len - pos + 1);
  dst[pos] = src;
}