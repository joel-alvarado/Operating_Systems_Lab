#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void TenSecondSleep() {
  float y = 5;
  float x = 450;
  for (int i = 0; i < 1000; i++) {
        usleep(10000);
    x = x + y / (5 * y * y);
  }
  printf("%f\n", x);
}

int main() {
  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_REALTIME, &start);
  TenSecondSleep();
  clock_gettime(CLOCK_REALTIME, &end);

  double elapsed =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

  printf("Elapsed time: %.9f seconds\n", elapsed);

  return 0;
}

//
//
//
//
//
//
//
//
//
//
// void TenSecondSleep2() {
//   float y = 0;
//   for (int i = 0; i < 10; i++) {
//     sleep(1);
//   }
// }

void TenSecondSleep3() {
  struct timespec time_data;

  time_data.tv_sec = 0;
  time_data.tv_nsec = 1000000;

  for (int i = 0; i < 1E4; i++) {
    nanosleep(&time_data, NULL);
  }
}
