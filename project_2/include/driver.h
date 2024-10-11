#ifndef IMU_DRIVER_INCLUDE_DRIVER_H
#define IMU_DRIVER_INCLUDE_DRIVER_H

/**
 * @brief Holds motion sensor data.
 *
 * @param time_elapsed Time (in seconds) since driver was started.
 * @param p_x Position of sensor in x-axis
 * @param p_y Position of sensor in y-axis
 * @param v_x Velocity of sensor in x-axis
 * @param v_y Velocity of sensor in y-axis
 * @param acc_x Acceleration of sensor in x-axis
 * @param acc_y Acceleration of sensor in y-axis
 *
 */
typedef struct sMotionSensorInfo MotionSensorInfo;

/**
 * @brief Starts the IMUDriver program. Sets a timer of 200ms to write current
 * MotionSensorInfo to `'data.txt'`. If `'data.txt'` already exists, latest
 * recorded values are loaded using `ReadLatestSensorData`. Else,
 * `GetDefaultSensorData` is used.
 *
 */
void StartIMUDriver();

/**
 * @brief Handler for SIGALRM. Will update and write current sensor data to
 * `data.txt`.
 *
 * @param signum Signal number recieved
 */
void SaveSensorData(int signum);

/**
 * @brief Calls `UpdateSensorInfo` with current sensor data and writes the
 * updated values to `'data.txt'`.
 *
 */
void UpdateSensorInfo();

/**
 * @brief Reads latest sensor info from "data.txt".
 *
 * @return `MotionSensorInfo*` with latest data read from "data.txt" NOTE: The
 * caller is responsible for freeing the pointer after use.
 */
MotionSensorInfo *ReadLatestSensorData();

/**
 * @brief Creates a `MotionSensorInfo *` with default values:
 * time_elapsed = 0,
 * p_x = 20,
 * p_y = 10,
 * v_x = 0,
 * v_y = 0,
 * acc_x = 0.15,
 * acc_y = 0.06
 *
 *
 * @return `MotionSensorInfo*` with default values. NOTE: The caller is
 * responsible for freeing the pointer after use.
 */
MotionSensorInfo *GetDefaultSensorData();

/**
 * @brief Exits gracefully, cleans up timer.
 *
 * @param signum Signal number recieved (15).
 */
void HandleSIGTERM(int signum);

#endif  // IMU_DRIVER_INCLUDE_DRIVER_H