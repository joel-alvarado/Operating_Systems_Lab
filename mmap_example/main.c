#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *file_path = argv[1];
  int fd = open(file_path, O_RDWR);
  if (fd == -1) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  struct stat file_stat;
  if (fstat(fd, &file_stat) == -1) {
    perror("Error getting file size");
    close(fd);
    exit(EXIT_FAILURE);
  }

  size_t file_size = file_stat.st_size;

  // Map the file into memory
  char *mapped_region =
      mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (mapped_region == MAP_FAILED) {
    perror("Error mapping file");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("File contents before modification:\n%s\n", mapped_region);

  // Modify the file through the mapped region
  const char *new_content = "New data!";
  size_t new_content_len = strlen(new_content);

  if (new_content_len <= file_size) {
    memcpy(mapped_region, new_content, new_content_len);
  } else {
    fprintf(stderr, "New content is larger than the file size!\n");
  }

  // Synchronize changes to the file
  if (msync(mapped_region, file_size, MS_SYNC) == -1) {
    perror("Error synchronizing file");
  }

  printf("File contents after modification:\n%s\n", mapped_region);

  // Unmap the memory and close the file
  if (munmap(mapped_region, file_size) == -1) {
    perror("Error unmapping memory");
  }

  close(fd);

  return 0;
}
