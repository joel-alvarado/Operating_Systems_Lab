#include "time_struct.h"

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

Time TimeInit(int hours, int minutes, int seconds) {
  Time t;
  t.hours = hours;
  t.minutes = minutes;
  t.seconds = seconds;
  return t;
}

void PrintTime(Time t) {
  printf("%02d:%02d:%02d\n", t.hours, t.minutes, t.seconds);
}

Time GetTime() {
  printf("Creating new time struct...\n");

  int user_hours;
  while (true) {
    printf("Hours: ");
    scanf("%d", &user_hours);
    if (user_hours >= 0 && user_hours <= 23) {
      break;
    } else {
      printf("Invalid hours: Range: (0 <= hours <= 23)\n");
    }
  }

  int user_minutes;
  while (true) {
    printf("Minutes: ");
    scanf("%d", &user_minutes);
    if (user_minutes >= 0 && user_minutes < 60) {
      break;
    } else {
      printf("Invalid minutes: Range: (0 <= minutes <= 60)\n");
    }
  }

  int user_seconds;
  while (true) {
    printf("Seconds: ");
    scanf("%d", &user_seconds);
    if (user_seconds >= 0 && user_seconds < 60) {
      break;
    } else {
      printf("Invalid seconds: Range: (0 <= seconds <= 60)\n");
    }
  }
  Time new_time;
  new_time.hours = user_hours;
  new_time.minutes = user_minutes;
  new_time.seconds = user_seconds;
  return new_time;
}