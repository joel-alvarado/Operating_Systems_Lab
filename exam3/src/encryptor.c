#include <stdio.h>
#include <stdlib.h>

int main() {
  // Read user input to encrypt
  char text[100];
  printf("Message to encrypt: ");
  fgets(text, sizeof(text), stdin);

  // Array to hold encrypted message
  int ciphertext[100];
  int size = EncryptData(text, ciphertext); // Encrypts data, returns amount of ints created
  SaveData(ciphertext, size); // Writes the ciphertext in binary to "file.dat"
  printf("Message encrypted as: ");
  for(int i = 0; i < size; i++) {
    printf("%d ", ciphertext[i]);
  }
  printf("\n");
  return 0;
}

int EncryptData(char *m, int *ciphertext) {
  int i = 0;
  while (1) {
    if (m[i] == '\n') { // Break on newline since fgets takes enter key as \n
      break;
    }
    ciphertext[i] = m[i] * 20 - 9; // Encrypts the current char
    i++;
  }
  return i; // Returns amount of ints that message was encrypted as
}

void SaveData(int *ciphertext, int size) {
  FILE *fd = fopen("file.dat", "wb");
  fwrite(ciphertext, sizeof(int), size, fd); // Write entire ciphertext to "file.dat"
  fclose(fd);
}