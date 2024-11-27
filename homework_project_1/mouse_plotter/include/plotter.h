#ifndef PLOTTER_H
#define PLOTTER_H

typedef struct {
  int x;
  int y;
} Point2D;

int StartMousePlotter();
void *DrawRoutine(void *args);
void LoadNextDataPoint();
int GetFileSize(const char *fname);
void HandlerTimerExpire(int sig);

#endif /* PLOTTER_H */