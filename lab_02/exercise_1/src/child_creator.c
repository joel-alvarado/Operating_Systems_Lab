#include "child_creator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void CreateChildren() {
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
  // Get current exec path and allow execution
  size_t binPathSize = 1024;
  char binPath[binPathSize];
  GetExecutablePath(binPath, binPathSize);
  // Concat the greetings exec into the current exec path
  strcat(&binPath, "/greetings");
  // Chmod greetings exec to allow execvp to run it
  AllowExecution(binPath);

  // Spawn 2 forks and turn into greetings exec
  char* args[] = {binPath, NULL};
  pid_t p1 = fork();
  if (p1 == 0) {
    execvp(args[0], args);
    perror("execvp failed");
    exit(EXIT_FAILURE);
  }
  pid_t p2 = fork();
  if (p2 == 0) {
    execvp(args[0], args);
    perror("execvp failed");
    exit(EXIT_FAILURE);
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
}

void PrintPid() { printf("I’m a child, and my id is: %d\n", getpid()); }

void GetExecutablePath(char* binPath, size_t size) {
  ssize_t len = readlink("/proc/self/exe", binPath, size - 1);

  if (len == -1) {
    perror("readlink failed");
    exit(EXIT_FAILURE);
  }

  binPath[len] = '\0';

  char* lastSlash = strrchr(binPath, '/');
  if (lastSlash != NULL) {
    *lastSlash = '\0';  // Null-terminate to keep only the directory part
  } else {
    fprintf(stderr, "Unexpected executable path format\n");
    exit(EXIT_FAILURE);
  }
}

void AllowExecution(char* binPath) {
  // Set execute permission to the file
  if (chmod(binPath, 0755) == -1) {
    perror("chmod failed");
    exit(EXIT_FAILURE);
  }
}