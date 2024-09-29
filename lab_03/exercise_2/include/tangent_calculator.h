#ifndef TANGENT_CALCULATOR_H
#define TANGENT_CALCULATOR_H

#include <math.h>
#include <pthread.h>
#include <stdio.h>

#define TANGENT_INPUT_ARRAY_SIZE 1000000

/**
 * @brief Contains data pertaining to a sumThreadRoutine thread
 * below.
 *
 */
typedef struct sThreadData tThreadData;

/**
 * @brief Calculates the sum of the tangent of the assigned chunk
 * `tThreadData.assigned_section` of `tThreadData.input_array`. Puts the
 * result in `tThreadData.result`.
 *
 * @param data `tThreadData` instance
 * @return void*
 */
void *sumThreadRoutine(void *data);

/**
 * @brief Fills the param `array` with the tangent
 * [1,`TANGENT_INPUT_ARRAY_SIZE`]
 *
 * @param array
 */
void fillTangentArray(double *array);

/**
 * @brief Launches the main sum program with multithreading. Will
 * launch 5 threads, each calculating the sum of a chunk of
 * `tangent_input_array`. Then these results are added to get the final
 * sum of `tangent_input_array` and are returned.
 *
 * @param tangent_input_array
 * @return double
 */
double launchSumWithMultithreading(double *tangent_input_array);

/**
 * @brief Launches the main sum program. Will sum each element of
 * `tangent_input_array` and return the result
 *
 * @param tangent_input_array
 * @return double
 */
double launchSum(double *tangent_input_array);

/**
 * @brief Compare the time taken for `launchSum` and
 * `launchSumWithMultithreading`
 *
 */
void compareTimeTaken();

#endif /* TANGENT_CALCULATOR */