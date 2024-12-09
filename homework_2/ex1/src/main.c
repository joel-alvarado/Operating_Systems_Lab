#define _POSIX_C_SOURCE 199309L

#include <curses.h> /* ncurses.h includes stdio.h */
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "plotter.h"

#define ENTER_NCURSES 10

int ball_x, ball_y;
int goal_x, goal_y;
int counter = 0;

int row, col;
int game_running = 1;

void CheckIfWon(void *args) {
  while (1) {
    if (ball_x == goal_x && ball_y == goal_y) {
        game_running = 0;
        sleep(1);
        mvprintw(50, 50, "You won!");
    }   
  }
}

void *RandomizeBallPos(void *args) {
  sleep(5);
  while (1) {
    goal_x = rand() % 101;
    goal_y = rand() % 26;

    if (goal_x > row) {
        goal_x = row;
    }
    if (goal_y > col) {
        goal_y = col;
    }

    sleep(3);
  }
}

void *DrawRoutine(void *args) {
  ball_x = 10;
  ball_y = 10;
  goal_x = 30;
  goal_y = 60;

  while (game_running) {
    // Draw all points
    erase();
    mvprintw(ball_y, ball_x, "o");
    mvprintw(goal_y, goal_x, "x");
    mvprintw(25, 0, "%d", counter);
    refresh();
    usleep(20000);
  }
}

int StartMousePlotter() {
  // Initialize ncurses
  initscr();
  getmaxyx(stdscr, row, col); /* Obtiene el numero de filas y columnas */
  
  pthread_t draw_tid;
  pthread_t update_ball_tid;
  pthread_t randomize_goal_tid;
  pthread_t game_won_

      pthread_create(&draw_tid, NULL, DrawRoutine, NULL);

  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  int key_stroke;
  int break_loop = 0;
  while (!game_running) {
    key_stroke = getch();
    switch (key_stroke) {
      case ENTER_NCURSES:
        break_loop = 1;  // Exit the loop
        break;
    }
  }

  // Clean up
  endwin();
  return 0;
}
