#ifndef SCHED_EXPERIMENT_H
#define SCHED_EXPERIMENT_H

/**
 * @brief Starts a task that hogs CPU. Counts to a very large number from 0, 30
 * times. Measures time taken to finish.
 *
 * @param id: The ID of the child process running.
 */
void CPUBoundTask(int id);

/**
 * @brief Start the scheduler experiment main function. Will run three child
 * processes with different scheduling priorities.
 *
 * @param int priorities[3]: The priorities of the 3 child processes
 *
 * @return int: Exit code
 */
int StartSchedulerExperiment(int priorities[3]);

#endif /* SCHED_EXPERIMENT_H */