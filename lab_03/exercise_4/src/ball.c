#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int x;
  int y;
} BallPosition;

typedef struct sThreadData {
  BallPosition ball_pos;
  int counter_val;
  int dx, dy;
  int running;
} tThreadData;

#define ENTER_NCURSES 10

void* updateBall(void* arg) {
  tThreadData* thread_data = (tThreadData*)arg;

  while (thread_data->running) {
    int row, col;
    getmaxyx(stdscr, row, col); /* Obtiene el numero de filas y columnas */

    clear();
    mvprintw(0, 0, "%d", thread_data->counter_val);
    mvprintw(thread_data->ball_pos.y, thread_data->ball_pos.x, "o");
    refresh();

    thread_data->ball_pos.x += thread_data->dx;
    thread_data->ball_pos.y += thread_data->dy;
    if (thread_data->ball_pos.x >= col - 1) {
      thread_data->dx = -1;
    }
    if (thread_data->ball_pos.y >= row - 1) {
      thread_data->dy = -1;
    }
    if (thread_data->ball_pos.x <= 1) {
      thread_data->dx = 1;
    }
    if (thread_data->ball_pos.y <= 1) {
      thread_data->dy = 1;
    }

    usleep(100000); /* Duerme por 100ms */
  }
  return NULL;
}

void* updateCounter(void* arg) {
  tThreadData* thread_data = (tThreadData*)arg;

  while (thread_data->running) {
    sleep(1); /* Duerme por 1 segundo */
    thread_data->counter_val++;
  }
  return NULL;
}

int startBouncingBall() {
  BallPosition ball_pos;
  int counter_val = 0;
  int dx, dy;
  int running = 1;
  ball_pos.x = 10;
  ball_pos.y = 20;
  dx = dy = 1;

  tThreadData thread_data;
  thread_data.ball_pos = ball_pos;
  thread_data.counter_val = counter_val;
  thread_data.dx = dx;
  thread_data.dy = dy;
  thread_data.running = running;

  pthread_t ball_thread, counter_thread;

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  pthread_create(&ball_thread, NULL, updateBall, &thread_data);
  pthread_create(&counter_thread, NULL, updateCounter, &thread_data);

  int key_stroke;
  int break_loop = 0;
  while (!break_loop) {
    key_stroke = getch();
    switch (key_stroke) {
      case KEY_LEFT:
        /* code */
        break;
      case 'f':
        /* code */
        break;
      case ENTER_NCURSES:
        break_loop = 1;  // Finaliza el bucle
        break;
    }
  }

  thread_data.running = 0;  // Detiene los hilos
  pthread_join(ball_thread, NULL);
  pthread_join(counter_thread, NULL);

  endwin();
  return 0;
}