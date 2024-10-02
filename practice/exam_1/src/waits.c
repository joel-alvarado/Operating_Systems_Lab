#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid == 0) {  // Child process
    printf("Child process executing...\n");
    sleep(2);  // Simulate some work
  } else {     // Parent process
    int status;
    waitpid(pid, &status, 0);  // Wait for the specific child to finish
    printf("Parent: Child has terminated with status %d\n",
           WEXITSTATUS(status));
  }
  return 0;
}
