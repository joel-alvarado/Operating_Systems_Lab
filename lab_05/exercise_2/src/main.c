#include "sched_experiment.h"

#define NUM_CHILDREN 3

int main() {
  int priorities1[NUM_CHILDREN] = {0, 0, 0};
  int priorities2[NUM_CHILDREN] = {-10, 0, 10};
  int priorities3[NUM_CHILDREN] = {-20, 0, 10};
  int priorities4[NUM_CHILDREN] = {0, 0, 20};
  int priorities5[NUM_CHILDREN] = {10, 10, -10};
  int priorities6[NUM_CHILDREN] = {-20, -20, -20};

  // Define an array of pointers to the priority arrays
  int* experiments[6] = {priorities1, priorities2, priorities3,
                         priorities4, priorities5, priorities6};

  for (int i = 0; i < 6; i++) {
    StartSchedulerExperiment(experiments[i]);
  }

  return 0;
}
