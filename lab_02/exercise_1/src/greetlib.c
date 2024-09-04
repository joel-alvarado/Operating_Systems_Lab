#include "greetlib.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void PrintGreetings() {
  pid_t p = getpid();
  printf("hello there, I’m process %d\n", p);
  sleep(10);
  printf("I’m done, bye!\n");
}