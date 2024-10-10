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
 * @brief Starts the IMUDriver program.
 *
 */
void StartIMUDriver();

/**
 * @brief Thread routine. Writes current `MotionSensorInfo` data to
 * `imu_data.txt`.
 *
 * @param sensor_info `void *` to an instance of `MotionSensorInfo`
 * @return `void *` to the thread's exit status.
 */
void *WriteDataRoutine(void *sensor_info);

/**
 * @brief Updates `sensor_info`. Increases time ellapsed by 200ms, and updates
 * position and velocity according to acceleration values.
 *
 * @param sensor_info `MotionSensorInfo *` containing sensor information to
 * update.
 */
void UpdateSensorInfo(MotionSensorInfo *sensor_info);

/**
 * @brief Reads latest sensor info from "data.txt".
 *
 * @return `MotionSensorInfo*` with latest data read from "data.txt"
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
 * NOTE: The caller is responsible for freeing the pointer after use.
 *
 * @return `MotionSensorInfo*` with default values read from "data.txt"
 */
MotionSensorInfo *GetDefaultSensorData();

#endif  // IMU_DRIVER_INCLUDE_DRIVER_H