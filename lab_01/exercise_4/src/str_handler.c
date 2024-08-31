#include "str_handler.h"

#include <string.h>

void Replace(char *src, char *dst, char find, char replacement) {
  strcpy(dst, src);
  int len = strlen(src);
  for (int i = 0; i < len; i++) {
    if (src[i] == find) {
      ReplaceAt(dst, replacement, i);
    }
  }
}

void Insert(char *src, char *dst, char find, char *ins) {
  strcpy(dst, src);
  for (int i = 0; i < strlen(dst); i++) {
    if (dst[i] == find) {
      InsertAt(dst, ins, i);
      i += strlen(ins);
    }
  }
}

void ReplaceAt(char *dst, char src, int pos) { dst[pos] = src; }

void InsertAt(char *dst, char *src, int pos) {
  char res[strlen(dst) + strlen(src) + 1];
  strncpy(res, dst, pos);
  res[pos] = '\0';
  strcat(res, src);
  strcat(res, dst + pos);
  strcpy(dst, res);
}