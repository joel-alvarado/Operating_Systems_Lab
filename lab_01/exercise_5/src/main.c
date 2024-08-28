#include <stdio.h>

#include "numlib.h"

int main() {
  char *res;
  for (int i = 0; i < 101; i++) {
    char *res2 = NumToStr(i, res);
    printf("Int: %d, String: %s\n", i, res2);
  }
}