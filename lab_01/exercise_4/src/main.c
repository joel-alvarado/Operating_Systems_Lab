#include <stdio.h>

#include "str_handler.h"

int main() {
  char src[] = "Hello World!";
  char dst[100];
  char dst2[100];

  printf("Before Replace() o with a: %s\n", src);
  Replace(&src, &dst, 'o', 'a');
  printf("After Replace(): %s\n", dst);

  printf("Before Insert() ooo at o: %s\n", src);
  Insert(&src, &dst2, 'o', "ooo");
  printf("After Insert(): %s\n", dst2);
}