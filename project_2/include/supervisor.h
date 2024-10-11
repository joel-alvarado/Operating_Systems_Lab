#ifndef IMU_DRIVER_INCLUDE_SUPERVISOR_H
#define IMU_DRIVER_INCLUDE_SUPERVISOR_H

#include <aio.h>

/**
 * @brief Starts supervisor process. Looks for a PID related to `process_name`.
 * If not found, fork execute `process_name`.
 *
 */
void StartSupervisor(char *process_name);

/**
 * @brief Get the PID from a process name.
 *
 * @param process_name Process name to find.
 * @return pid_t PID of `process_name`, -1 if not found.
 */
pid_t GetPidByName(const char *process_name);

/**
 * @brief Checks if `process_name` is running. If not, calls
 * `StartNewSupervisedProcess`.
 *
 * @param signum Signal number recieved (SIGUSR1 from timer).
 */
void CheckAndRestartIMUDriver(int signum);

/**
 * @brief Fork-executes a new `process_name` process.
 *
 * @return pid_t PID of the new `process_name` process.
 */
pid_t StartNewSupervisedProcess();

/**
 * @brief Handles `SIGTERM` signal to exit gracefully.
 *
 * @param signum Signal recieved (SIGTERM)
 */
void HandleSIGTERM(int signum);

#endif /* IMU_DRIVER_INCLUDE_SUPERVISOR_H */