#include "child_creator.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int CreateChildren(int n) {
  pid_t p1 = fork();
  if (p1 == 0) {
    PrintPid();
    SpawnGreetingsForks();
    exit(0);
  }
  pid_t p2 = fork();
  if (p2 == 0) {
    PrintPid();
    SpawnGreetingsForks();
    exit(0);
  }
  pid_t p3 = fork();
  if (p3 == 0) {
    PrintPid();
    SpawnGreetingsForks();
    exit(0);
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
  waitpid(p3, NULL, 0);
}

void SpawnGreetingsForks() {
  pid_t p1 = fork();
  char* args[] = {"/home/os_100_1/os_labs/lab_02/exercise_1/build/greetings",
                  NULL};
  if (p1 == 0) {
    execlp(args[0], args);
  }
  pid_t p2 = fork();
  if (p2 == 0) {
    execlp(args[0], args);
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
}

void PrintPid() { printf("I’m a child, and my id is: %d\n", getpid()); }