#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
#define MAX_BALLS 10

// Global array for up to 10 ball threads
tThreadData ball_data[MAX_BALLS];
pthread_t ball_threads[MAX_BALLS];
pthread_t draw_thread;
pthread_t counter_thread;
int active_balls = 0;
int counter_val = 0;
pthread_mutex_t screen_lock;  // Mutex for screen updates

void* updateBallPosition(void* arg) {
  tThreadData* thread_data = (tThreadData*)arg;

  while (thread_data->running) {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Boundary check on edges of the screen
    if (thread_data->ball_pos.x + thread_data->dx >= col - 1 ||
        thread_data->ball_pos.x + thread_data->dx <= 0) {
      thread_data->dx = -thread_data->dx;
    }
    if (thread_data->ball_pos.y + thread_data->dy >= row - 1 ||
        thread_data->ball_pos.y + thread_data->dy <= 0) {
      thread_data->dy = -thread_data->dy;
    }

    // Update position
    thread_data->ball_pos.x += thread_data->dx;
    thread_data->ball_pos.y += thread_data->dy;

    usleep(100000);
  }
  return NULL;
}

void* drawScreen(void* arg) {
  while (1) {
    // Lock the screen before drawing and clear
    pthread_mutex_lock(&screen_lock);
    clear();

    // Draw on counter at the top left corner
    mvprintw(0, 0, "%d", counter_val);

    // Draw all balls
    for (int i = 0; i < active_balls; i++) {
      mvaddch(ball_data[i].ball_pos.y, ball_data[i].ball_pos.x, 'o');
    }

    // Refresh the screen & unlock the screen after drawing
    refresh();
    pthread_mutex_unlock(&screen_lock);

    usleep(5000);  // Redraw every 5ms
  }
  return NULL;
}

void* updateCounter(void* arg) {
  while (1) {
    sleep(1);
    pthread_mutex_lock(&screen_lock);
    counter_val++;
    pthread_mutex_unlock(&screen_lock);
  }
  return NULL;
}

void createNewBall() {
  if (active_balls < MAX_BALLS) {
    tThreadData* new_ball = &ball_data[active_balls];

    // Random starting position
    int row, col;
    getmaxyx(stdscr, row, col);
    new_ball->ball_pos.x = rand() % (col - 2) + 1;
    new_ball->ball_pos.y = rand() % (row - 2) + 1;
    new_ball->dx = (rand() % 2 == 0) ? 1 : -1;
    new_ball->dy = (rand() % 2 == 0) ? 1 : -1;
    new_ball->running = 1;

    pthread_create(&ball_threads[active_balls], NULL, updateBallPosition,
                   new_ball);
    active_balls++;
  }
}

void changeBallDirection() {
  for (int i = 0; i < active_balls; i++) {
    ball_data[i].dx = -ball_data[i].dx;
    ball_data[i].dy = -ball_data[i].dy;
  }
}

int startBouncingBall() {
  srand(time(NULL));

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  pthread_mutex_init(&screen_lock, NULL);

  // Create initial ball
  createNewBall();

  pthread_create(&draw_thread, NULL, drawScreen, NULL);
  pthread_create(&counter_thread, NULL, updateCounter, NULL);

  int key_stroke;
  int break_loop = 0;

  while (!break_loop) {
    key_stroke = getch();
    switch (key_stroke) {
      case ' ':
        createNewBall();
        break;
      case 'c':
        changeBallDirection();
        break;
      case ENTER_NCURSES:
        break_loop = 1;
        break;
    }
  }

  // End all ball threads
  for (int i = 0; i < active_balls; i++) {
    ball_data[i].running = 0;
    pthread_join(ball_threads[i], NULL);
  }

  // Stop the drawing and counter threads
  pthread_cancel(draw_thread);
  pthread_cancel(counter_thread);
  pthread_join(draw_thread, NULL);
  pthread_join(counter_thread, NULL);

  endwin();
  return 0;
}
