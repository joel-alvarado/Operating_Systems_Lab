#include "saver.h"

#include <X11/Xlib.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int finished_reading = 0;
Point2D *current_mouse_position;
int min_x, max_x, min_y, max_y;
float scale_x, scale_y;

int screen_width;
int screen_height;

void HandleSIGINT(int sig) { finished_reading = 1; }

void StartMouseSaver() {
  signal(SIGINT, HandleSIGINT);

  // Uses libx11-dev to get current screen dimensions
  Screen *screen = GetSystemScreen();
  if (!screen) {
    fprintf(stderr, "Error reading screen data.\n");
    return;
  }
  screen_width = screen->width;
  screen_height = screen->height;
  printf("Detected screen with dimensions %dx%d\n", screen_width,
         screen_height);

  printf("Capturing mouse position... (Use Ctrl+C to finish.)\n");

  // Set current mouse position to center of the screen
  current_mouse_position = malloc(sizeof(Point2D));
  current_mouse_position->x =
      screen_width / 2;  // Kinda ghetto but i couldnt find a way to get current
                         // cursor position, oh well
  current_mouse_position->y = screen_height / 2;  // Same here

  // Initialize min and max positions
  min_x = current_mouse_position->x;
  max_x = current_mouse_position->x;
  min_y = current_mouse_position->y;
  max_y = current_mouse_position->y;

  ReadWriteMouseData();

  // Get range of coords gathered from data
  float range_x = max_x - min_x;
  float range_y = max_y - min_y;
  if (range_x == 0) range_x = 1;  // Prevents divison by 0 ( likely wont happen)
  if (range_y == 0) range_y = 1;

  // Calculate factor to scale all coordinates to terminal size
  scale_x = 100.0f / range_x;
  scale_y = 25.0f / range_y;

  ScaleMouseData();
  free(current_mouse_position);  // Kinda useless but good practice

  printf(
      "Done. Terminal-scaled data can be found in the current directory in "
      "file 'mouse_data.dat'.\n");
}

void ReadWriteMouseData() {
  FILE *mouse_input_fd = fopen("/dev/input/mice", "rb");  // Read mouse delta
  if (!mouse_input_fd) {
    fprintf(stderr, "Failed to open /dev/input/mice for reading.\n");
    fprintf(stderr, "Perhaphs try running as root?\n");
    exit(EXIT_FAILURE);
  }

  FILE *mouse_data_file = fopen("mouse_data.dat", "wb");
  if (!mouse_data_file) {
    fprintf(stderr, "Failed to open mouse_data.dat for writing.\n");
    exit(EXIT_FAILURE);
  }

  unsigned char buffer[3];  // unsigned char for raw bytes

  // Read delta, add to current position and write to mouse_data.dat
  while (!finished_reading) {
    if (fread(buffer, 1, 3, mouse_input_fd) != 3) {
      fprintf(stderr, "Failed to read mouse input.\n");
      break;
    }

    MouseDelta current_delta;
    current_delta.delta_x = (int8_t)buffer[1];
    current_delta.delta_y = (int8_t)buffer[2];

    current_mouse_position->x += current_delta.delta_x;
    current_mouse_position->y -=
        current_delta.delta_y;  // Invert delta_y since the OS takes positive y
                                // as going down

    // Keep position within screen bounds
    if (current_mouse_position->x < 0) {
      current_mouse_position->x = 0;
    }
    if (current_mouse_position->y < 0) {
      current_mouse_position->y = 0;
    }
    if (current_mouse_position->x >= screen_width) {
      current_mouse_position->x = screen_width - 1;
    }
    if (current_mouse_position->y >= screen_height) {
      current_mouse_position->y = screen_height - 1;
    }

    // Update min and max positions for future terminal scaling
    if (current_mouse_position->x < min_x) {
      min_x = current_mouse_position->x;
    }
    if (current_mouse_position->x > max_x) {
      max_x = current_mouse_position->x;
    }
    if (current_mouse_position->y < min_y) {
      min_y = current_mouse_position->y;
    }
    if (current_mouse_position->y > max_y) {
      max_y = current_mouse_position->y;
    }

    // Write current pixel positions to file
    int coords[2] = {current_mouse_position->x, current_mouse_position->y};
    if (fwrite(coords, sizeof(int), 2, mouse_data_file) != 2) {
      fprintf(stderr, "Failed to write to mouse_data.dat.\n");
      break;
    }
  }

  fclose(mouse_input_fd);
  fclose(mouse_data_file);
}

void ScaleMouseData() {
  FILE *mouse_data_file =
      fopen("mouse_data.dat", "rb+");  // Open in read/write mode
  if (!mouse_data_file) {
    fprintf(stderr, "Failed to open mouse_data.dat for rescaling.\n");
    return;
  }

  int coords[2];
  while (fread(coords, sizeof(int), 2, mouse_data_file) == 2) {
    // Calculate terminal coordinates
    float terminal_x = (coords[0] - min_x) * scale_x;
    float terminal_y = (coords[1] - min_y) * scale_y;

    // Convert to integers with rounding
    int scaled_x = (int)(terminal_x + 0.5f);
    int scaled_y = (int)(terminal_y + 0.5f);
    int scaled_coords[2] = {scaled_x, scaled_y};

    // Move file pointer to overwrite coordinates read
    if (fseek(mouse_data_file, -2 * sizeof(int), SEEK_CUR) != 0) {
      fprintf(stderr, "Failed to seek in mouse_data.dat.\n");
      break;
    }

    // Write scaled coordinates to file
    if (fwrite(scaled_coords, sizeof(int), 2, mouse_data_file) != 2) {
      fprintf(stderr, "Failed to write scaled data to mouse_data.dat.\n");
      break;
    }

    // Flush to ensure data is written
    fflush(mouse_data_file);
  }
  fclose(mouse_data_file);
}

// For debugging purposes
void PrintMouseData() {
  FILE *mouse_data_file = fopen("mouse_data.dat", "rb");
  if (!mouse_data_file) {
    fprintf(stderr, "Failed to open mouse_data.dat for reading.\n");
    return;
  }

  int coords[2];
  while (fread(coords, sizeof(int), 2, mouse_data_file) == 2) {
    printf("Coords read: (%d, %d)\n", coords[0], coords[1]);
  }

  fclose(mouse_data_file);
}

Screen *GetSystemScreen() {
  // Open connection to X server
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Unable to open X display\n");
    return NULL;
  }

  // Get current screen
  Screen *screen = DefaultScreenOfDisplay(display);

  XCloseDisplay(display);
  return screen;
}
