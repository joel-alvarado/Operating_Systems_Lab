#ifndef SAVER_H
#define SAVER_H

typedef struct MouseDelta {
  int delta_x;
  int delta_y;
} MouseDelta;

void StartMouseSaver();
void *WriteMouseDataRoutine(void *args);
MouseDelta *GetMousePosition();
MouseDelta *RelativeToAbsolutePos(MouseDelta *delta);

#endif /* SAVER_H */