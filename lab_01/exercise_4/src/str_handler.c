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
  for (int i = 0; i < strlen(src); i++) {
    if (src[i] == find) {
      InsertAt(dst, ins, i);
    }
  }
}

void ReplaceAt(char *dst, char src, int pos) { dst[pos] = src; }

void InsertAt(char *dst, char *src, int pos) {
  int dst_len = strlen(dst);
  int src_len = strlen(src);
  if (pos > dst_len) {
    return;
  }
  char temp[src_len];
  strcpy();
  memmove(dst + pos + src_len, dst + pos, dst_len - pos + 1);
  strcpy(dst + pos, src);
}