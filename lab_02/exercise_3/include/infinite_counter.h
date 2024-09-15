#ifndef INFINITE_COUNTER_H
#define INFINITE_COUNTER_H

int total = 0;
int step = 1;

/**
 * @brief Handler for SIGUSR1. Resets counter to 0.
 *
 * @param sig
 */
void handle_sigusr1(int sig);

/**
 * @brief Handler for SIGURS2. Changes the direction (step) of the counter.
 *
 * @param sig
 */
void handle_sigusr2(int sig);

/**
 * @brief Handler for SIGTERM. Exits the current process.
 *
 * @param sig
 */
void handle_sigterm(int sig);

#endif /* INFINITE_COUNTER_H */