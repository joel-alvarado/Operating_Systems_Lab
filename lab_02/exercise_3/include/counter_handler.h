#ifndef COUNTER_HANDLER_H
#define COUNTER_HANDLER_H

int *local_array;
int process_count;
int killed;

/**
 * @brief Prints the PIDs that exist in the shared memeory of spawn_counters and
 * counter_handler (format [PID1, PID2, -1]) (-1 for killed PID)
 *
 */
void print_pids();

/**
 * @brief Read PID nunmber from user and return it.
 *
 * @return int
 */
int read_pid();

#endif /* COUNTER_HANDLER_H */