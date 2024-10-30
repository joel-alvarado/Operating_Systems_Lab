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
 * @return int: Exit code
 */
int StartSchedulerExperiment();

#endif /* SCHED_EXPERIMENT_H */