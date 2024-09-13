#include <stdio.h>

int main(int argc, char **argv) {
  // code
  print_counter_pids(argc, argv);
}

void print_counter_pids(int count, char **pids) {
  for (int i = 0; i < count; i++) {
    printf("[%s,", pids[i]);
  }
  printf("]\n");
}