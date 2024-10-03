#!/bin/bash
DIRECTORY=deliverables
if [ ! -d "$DIRECTORY" ]; then
  mkdir "$DIRECTORY"
fi
zip -r deliverables/ex1.zip exercise_1/include exercise_1/src exercise_1/CMakeLists.txt
zip -r deliverables/ex2.zip exercise_2/include exercise_2/src exercise_2/CMakeLists.txt
zip -r deliverables/ex3.zip exercise_3/include exercise_3/src exercise_3/CMakeLists.txt
zip -r deliverables/ex4.zip exercise_4/include exercise_4/src exercise_4/CMakeLists.txt
zip -r deliverables/ex5.zip exercise_4/include exercise_4/src exercise_4/CMakeLists.txt