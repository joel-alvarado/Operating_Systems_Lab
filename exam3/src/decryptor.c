#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main() {
  char text[100];
  int ciphertext[100];

  int read_ints = LoadData(ciphertext); // Loads binary data from "file.dat" (composed of ints)
  DecryptData(ciphertext, text, read_ints); // Decrypts message from read ints
  printf("Decrypted message: %s\n", text);
  return 0;
}

void DecryptData(int *ciphertext, char *m, int size) {
  for(int i = 0; i < size; i++) {
    m[i] = (ciphertext[i] + 9) / 20; // Reverses encrypt with k1 as 20 and k2 as 9
  }
  m[size] = '\0'; // Add null terminator for printing
}

int LoadData(int *ciphertext) {
  FILE *fd = fopen("file.dat", "rb");
  struct stat st;
  stat("file.dat", &st);
  long size = st.st_size; // Gets the file size in bytes
  fread(ciphertext, sizeof(int), size, fd); // Read binary contents into ciphertext
  fclose(fd);
  return size / sizeof(int); // Returns amount of ints read (if 20 bytes => 5 ints)
}