#ifndef PLOTTER_H
#define PLOTTER_H

typedef struct {
  int x;
  int y;
} Point2D;

/**
 * @brief Starts the mouse plotting application.
 *
 * This function initializes the resources, sets up ncurses, timers,
 * and threads required for the application. It captures and plots points
 * from the "mouse_data.dat" file until the user exits by pressing Enter.
 *
 * @return 0 on success, -1 on failure.
 */
int StartMousePlotter();

/**
 * @brief Thread routine for drawing points on the screen.
 *
 * Continuously locks the mutex to access and plot all currently loaded points
 * onto the ncurses window. It redraws the screen at regular intervals.
 *
 * @param args Pointer to arguments for the thread (unused in this function).
 * @return Always returns NULL.
 */
void *DrawRoutine(void *args);

/**
 * @brief Loads the next data point from the "mouse_data.dat" file.
 *
 * Reads the next set of coordinates from the file and allocates memory for a
 * `Point2D` structure. Updates the `plotted_points` array with the new point.
 */
void LoadNextDataPoint();

/**
 * @brief Retrieves the size of a file in bytes.
 *
 * Opens the specified file in binary mode and determines its size by seeking
 * to the end of the file.
 *
 * @param fname Path to the file whose size is to be determined.
 * @return File size in bytes, or 0 if the file could not be opened.
 */
int GetFileSize(const char *fname);

/**
 * @brief Signal handler for the timer expiration.
 *
 * This handler is invoked when the timer expires. It locks the mutex,
 * loads the next data point if available, and unlocks the mutex.
 *
 * @param sig Signal number (always SIGALRM in this application).
 */
void HandlerTimerExpire(int sig);

#endif /* PLOTTER_H */
