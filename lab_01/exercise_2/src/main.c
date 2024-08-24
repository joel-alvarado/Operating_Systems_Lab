#include <stdio.h>

#include "time_struct.h"

int main() {
  Time initial_time = TimeInit(10, 27, 34);
  printf("Initial time: ");
  PrintTime(initial_time);

  Time user_time = GetTime();
  printf("New time: ");
  PrintTime(user_time);
}