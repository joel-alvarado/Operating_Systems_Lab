#include <stdio.h>

#include "time_struct.h"

int main() {
  Time initial_time = TimeInitSystemTime();
  printf("Initial time: ");
  PrintTime(initial_time);

  Time user_time = GetTime();
  printf("New time: ");
  PrintTime(user_time);
}