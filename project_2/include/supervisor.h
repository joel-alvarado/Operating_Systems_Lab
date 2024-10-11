#ifndef IMU_DRIVER_INCLUDE_SUPERVISOR_H
#define IMU_DRIVER_INCLUDE_SUPERVISOR_H

#include <aio.h>

/**
 * @brief Starts supervisor process. Looks for a PID related to `imu_driver`. If
 * not found, fork execute the imu_driver process.
 *
 */
void StartIMUSupervisor();

/**
 * @brief Get the PID from a process name.
 *
 * @param process_name Process name to find.
 * @return pid_t PID of `process_name`, -1 if not found.
 */
pid_t GetPidByName(const char *process_name);

/**
 * @brief Checks if imu_driver is running. If not, calls
 * `StartNewIMUDriverProcess`.
 *
 * @param signum Signal number recieved (SIGUSR1 from timer).
 */
void CheckAndRestartIMUDriver(int signum);

/**
 * @brief Fork-executes a new `imu_driver` process.
 *
 * @return pid_t PID of the new `imu_driver` process.
 */
pid_t StartNewIMUDriverProcess();

/**
 * @brief Handles `SIGTERM` signal to exit gracefully.
 *
 * @param signum Signal recieved (SIGTERM)
 */
void HandleSIGTERM(int signum);

#endif