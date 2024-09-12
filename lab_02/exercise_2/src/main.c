#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "termlib.h"

#define HOST_NAME_MAX 128

typedef struct {
  char *user;
  char *hostname;
  char *cwd;
} shinfo;

shinfo *getupdatedinf() {
  shinfo *info = malloc(sizeof(shinfo));

  // Set up current user and hostname
  struct passwd *p = getpwuid(getuid());
  char hostname[HOST_NAME_MAX + 1];
  gethostname(hostname, sizeof(hostname));

  // Set up current working directory
  long size;
  char *buf;
  char *cwd;
  size = pathconf(".", _PC_PATH_MAX);
  if ((buf = (char *)malloc((size_t)size)) != NULL)
    cwd = getcwd(buf, (size_t)size);

  info->user = strdup(p->pw_name);
  info->hostname = strdup(hostname);
  info->cwd = strdup(cwd);
}

int main() {
  printf(
      "Welcome to simple shell!\n"
      "Most of the basic bash commands work here.\n"
      "Things that do not work: environment variables, ~ operator, pipe "
      "operators (|| > etc)\n"
      ""
      "Program execution is also enabled. Use syntax ./<bin>\n"
      "You can use 'cd' to exit out of the build directory.\n");
  while (1) {
    // Set up prefix for shell
    shinfo *info = getupdatedinf();
    fprintf(stdout, "%s@simpsh:%s$ ", info->user, info->cwd);

    char buffer[INPUT_MAX_LEN + 2];  // +1 for \n & +1 for \0
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      // Remove newline if present
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
      }
    } else {
      printf("Error reading input.");
      exit(EXIT_FAILURE);
    }

    // Allocate memory for input commands
    char **commands = malloc(MAX_CMD_AMOUNT * sizeof(char *));
    for (int i = 0; i < MAX_CMD_AMOUNT; i++) {
      commands[i] = malloc(MAX_CMD_LENGTH * sizeof(char));
    }

    // Split all commands at delimiter ;
    ssplit(commands, MAX_CMD_AMOUNT, buffer, ';');

    // Remove leading/trailing whitespaces from each command
    for (int i = 0; commands[i] != NULL; i++) {
      strip(commands[i], commands[i]);
    }

    // Individually fork and execvp each command
    for (int i = 0; commands[i] != NULL; i++) {
      // Allocate mem for each command and its flags
      char **args = malloc(MAX_CMD_FLAG_AMOUNT * sizeof(char *));
      for (int i = 0; i < MAX_CMD_FLAG_AMOUNT; i++) {
        args[i] = malloc(MAX_CMD_FLAG_LENGTH * sizeof(char));
      }

      // Split each command at whitespace
      ssplit(args, MAX_CMD_FLAG_AMOUNT, commands[i], ' ');

      // This allows cd to actually work since cd is a syscall not a program
      if (strcmp(args[0], "cd") == 0) {
        if (chdir(args[1]) == -1) {
          perror(args[0]);
        }
        continue;
      }

      // Execute the command through fork and execvp
      int p = fork();
      if (p == 0) {
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
      }
      waitpid(p, NULL, 0);

      // Free args buffer, no need to free each element since ssplit uses the
      // pointer from the original string
      free(args);
    }
  }
}