#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define PAGE_SIZE 4096

int left_idx;
int right_idx;

int choice;

char *file_block;

void *LeftThread(void *arg) {
  while (left_idx < right_idx) {
    if (choice == 0) {
      file_block[left_idx] = toupper(file_block[left_idx]);
    } else {
      file_block[left_idx] = tolower(file_block[left_idx]);
    }
    left_idx++;
  }
}

void *RightThread(void *arg) {
  while (left_idx < right_idx) {
    if (choice == 0) {
      file_block[right_idx] = toupper(file_block[right_idx]);
    } else {
      file_block[right_idx] = tolower(file_block[right_idx]);
    }
    right_idx--;
  }
}

int main() {
  int fd = open("./lorem_ipsum.txt", O_RDWR, S_IRUSR | S_IWUSR);

  struct stat sb;

  fstat(fd, &sb);

  char *file_block =
      mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (file_block == -1) {
    perror("error in mmap");
    exit(0);
  }

  left_idx = 0;
  right_idx = sb.st_size;

  scanf("%d", &choice);

  pthread_t left_tid;
  pthread_t right_tid;

  pthread_create(&left_tid, NULL, LeftThread, NULL);
  pthread_create(&right_tid, NULL, RightThread, NULL);
  pthread_join(left_tid, NULL);
  pthread_join(right_tid, NULL);
}