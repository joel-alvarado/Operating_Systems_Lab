#ifndef IMU_DRIVER_INCLUDE_SUPERVISOR_H
#define IMU_DRIVER_INCLUDE_SUPERVISOR_H

#include <aio.h>

void StartIMUSupervisor();
pid_t GetPidByName(const char *process_name);
void CheckAndRestartIMUDriver(int signum);
pid_t StartNewIMUDriverProcess();
void HandleSIGTERM(int signum);

#endif