#ifndef SAVER_H
#define SAVER_H

#include <X11/Xlib.h>

typedef struct {
  int8_t delta_x;
  int8_t delta_y;
} MouseDelta;

typedef struct {
  int x;
  int y;
} Point2D;

void StartMouseSaver();
void ReadWriteMouseData();
void ScaleMouseData();
void PrintMouseData();
Screen *GetSystemScreen();

#endif /* SAVER_H */