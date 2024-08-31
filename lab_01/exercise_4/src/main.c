#include <stdio.h>

#include "str_handler.h"

int main() {
  char src[] = "Hello World!";
  char dst[100];

  //   printf("Before Replace(): %s\n", src);
  //   Replace(&src, &dst, 'o', 'a');
  //   printf("After Replace(): %s\n", dst);

  printf("Before Insert(): %s\n", src);
  Insert(&src, &dst, 'o', "uwu");
  printf("After Insert(): %s\n", dst);
}