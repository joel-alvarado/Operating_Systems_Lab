#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();  // Create a child process

  if (pid == 0) {  // Child process
    printf("This is the child process with PID: %d\n", getpid());
  } else if (pid > 0) {  // Parent process
    printf("This is the parent process with PID: %d, child's PID: %d\n",
           getpid(), pid);
  } else {  // Error
    perror("fork failed");
  }
  return 0;
}
