#include <stdio.h>
#include <unistd.h>

int main() {
  char *args[] = {"/bin/ls", "-l", NULL};  // Command to execute

  if (fork() == 0) {          // Child process
    execvp(args[0], args);    // Replace child process with `ls -l` command
    perror("execvp failed");  // Only reached if exec fails
  } else {                    // Parent process
    wait(NULL);               // Wait for the child to finish
    printf("Child process completed\n");
  }
  return 0;
}
