#include "numlib.h"

#include <stdlib.h>
#include <string.h>

char *NumToStr(int num, char *str) {
  char *num_strings[] = {"zero", "one", "two",   "three", "four",
                         "five", "six", "seven", "eight", "nine"};

  // Calculate length of digit
  int num_len = NumLen(num);

  if (num_len == 1) {
    char *new_str = (char *)malloc(sizeof(num_strings[num]));
    new_str = num_strings[num];
    return new_str;
  }

  // Store each digit of num separately
  // Also calculate total bytes needed for the final string
  int total_bytes_needed = 0;
  int digit = num;
  int separated_digits[num_len];
  for (int i = num_len - 1; i >= 0; i--) {
    int last_digit = digit % 10;
    separated_digits[i] = last_digit;
    total_bytes_needed += strlen(num_strings[last_digit]);
    digit /= 10;
  }

  int spaces_needed = num_len - 1;
  char *new_str = (char *)malloc(total_bytes_needed + spaces_needed + 1);
  for (int i = 0; i < num_len; i++) {
    int current_digit = separated_digits[i];
    char *verbal_digit = num_strings[current_digit];
    new_str = strcat(new_str, verbal_digit);
    if (i != num_len - 1) {
      new_str = strcat(new_str, " ");
    }
  }
  return new_str;
}

int NumLen(int num) {
  int digit = num;
  int num_len = 1;
  while (digit > 9) {
    digit /= 10;
    num_len++;
  }
  return num_len;
}