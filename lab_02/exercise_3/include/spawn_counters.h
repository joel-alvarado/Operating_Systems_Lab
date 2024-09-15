#ifndef SPAWN_COUNTERS_H
#define SPAWN_COUNTERS_H

int *shared_array;
int counter_amount;
int killed;

/**
 * @brief Handler for SIGCHLD. When a child of this process dies, increment
 * killed variable;
 *
 * @param sign
 */
void handle_sigchild(int sign);

#endif /* SPAWN_COUNTERS_H */