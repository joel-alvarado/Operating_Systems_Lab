#include "saver.h"

#include <stdio.h>
#include <unistd.h>

void StartMouseSaver() {
  FILE *mouse_fd = fopen("/dev/input/mice", "rb");
  unsigned char buffer[3];
  while (1) {
    fread(buffer, 1, 3, mouse_fd);
    printf("%d, %d\n", buffer[1], buffer[2]);
    sleep(1);
  }
}

void *WriteMouseDataRoutine(void *args) {}
MouseDelta *GetMousePosition() {}
MouseDelta *RelativeToAbsolutePos(MouseDelta *delta) {}