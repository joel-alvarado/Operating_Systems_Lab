#include "plotter.h"

#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define ENTER_NCURSES 10

Point2D plotted_points;

void DrawRoutine() {
  int row, col;
  getmaxyx(stdscr, row, col); /* Obtiene el numero de filas y columnas */

  clear();
  mvprintw(0, 0, "%d", "0");
  refresh();
}

int StartMousePlotter() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

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