#include "sched_experiment.h"

#define NUM_CHILDREN 3

int main() {
  int priorities1[NUM_CHILDREN] = {-10, 0, 10};

  // Define an array of pointers to the priority arrays
  int* experiments[1] = {priorities1};
  StartSchedulerExperiment(experiments[0]);

  return 0;
}
