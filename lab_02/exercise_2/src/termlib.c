#include "termlib.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ssplit(char **dst, size_t size, char *src, char split) {
  char delimiter[2] = {split, '\0'};
  char *token;
  size_t i = 0;

  token = strtok(src, delimiter);  // First token
  while (token != NULL && i < size) {
    dst[i] = token;  // Store the token in the destination array
    i++;
    token = strtok(NULL, delimiter);  // Get next token
  }

  // Fill the remaining slots with NULL if fewer tokens are found
  while (i < size) {
    dst[i] = NULL;
    i++;
  }
}

void strip(char *dst, char *src) {
  char *start = src;
  char *end;

  // Trim leading whitespace
  while (isspace(*start)) {
    start++;
  }

  // Trim trailing whitespace
  end = start + strlen(start) - 1;
  while (end > start && isspace(*end)) {
    end--;
  }

  size_t len = end - start + 2;
  memcpy(dst, start, len);
}