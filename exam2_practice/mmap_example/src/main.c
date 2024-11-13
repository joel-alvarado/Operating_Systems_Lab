#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE 4096

int main() {
  int* first_block = mmap(0x35000, 3 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  int* second_block = mmap(0x40000, 3 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  int* third_block = mmap(0x45000, 17, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);  // file,offset

  int* malloc_block;
  for (int i = 0; i < 8; i++) {
    malloc_block = malloc(PAGE_SIZE);
  }

  third_block[100] = 5;

  printf("%lx\n", (first_block));

  printf("%lx\n", (second_block));

  printf("%lx\n", (third_block));

  printf("%lx\n", (malloc_block));

  // Free the allocated memory
  if (munmap(first_block, 3 * PAGE_SIZE) == -1) {
    perror("munmap failed");
    return 1;
  }

  sleep(1000);

  return 0;
}
