#include <stdio.h>
#include <string.h>

#include "supervisor.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: supervisor <executable>\n");
    return 1;
  }
  StartSupervisor(argv[1]);
  return 0;
}