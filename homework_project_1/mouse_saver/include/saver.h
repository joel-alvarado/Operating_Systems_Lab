#ifndef SAVER_H
#define SAVER_H

#include <X11/Xlib.h>

typedef struct {
  int x;
  int y;
} Point2D;

/**
 * @struct MouseDelta
 * @brief Represents changes in mouse coordinates.
 */
typedef struct {
  int8_t delta_x;
  int8_t delta_y;
} MouseDelta;

/**
 * @brief Starts the mouse saver program.
 *
 * Initializes signal handling, determines the screen dimensions, and captures
 * mouse movement data from `/dev/input/mice`. Scales the data for terminal
 * display and writes the output to `mouse_data.dat`.
 */
void StartMouseSaver();

/**
 * @brief Reads mouse delta data and writes absolute coordinates.
 *
 * Captures mouse movement data from `/dev/input/mice` in raw format, converts
 * it to absolute coordinates, and writes the data to `mouse_data.dat`. Updates
 * minimum and maximum coordinates for scaling purposes.
 */
void ReadWriteMouseData();

/**
 * @brief Scales captured mouse data to fit terminal dimensions.
 *
 * Reads the raw coordinates from `mouse_data.dat`, scales them to fit within
 * a 100x25 grid, and overwrites the original file with the scaled coordinates.
 */
void ScaleMouseData();

/**
 * @brief Prints the contents of `mouse_data.dat` for debugging purposes.
 *
 * Reads and displays the scaled coordinates stored in `mouse_data.dat`.
 */
void PrintMouseData();

/**
 * @brief Retrieves the system screen using X11.
 *
 * Uses the X11 library to determine the dimensions of the current screen.
 *
 * @return Pointer to a `Screen` structure containing screen details, or NULL
 *         if the screen could not be retrieved.
 */
Screen *GetSystemScreen();

/**
 * @brief Signal handler for SIGINT.
 *
 * Sets the `finished_reading` flag to 1 to terminate the mouse data reading
 * loop.
 *
 * @param sig Signal number (expected to be SIGINT).
 */
void HandleSIGINT(int sig);

#endif /* SAVER_H */