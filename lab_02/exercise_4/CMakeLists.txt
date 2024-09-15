cmake_minimum_required(VERSION 3.5.0)
project(exercise_4 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(exercise_4
    src/main.c
)

add_executable(shm_write_numbers
    src/shm_write_numbers.c
)

add_executable(shm_sum_numbers
    src/shm_sum_numbers.c
)

add_executable(msgq_write_numbers
    src/msgq_write_numbers.c
)

add_executable(msgq_sum_numbers
    src/msgq_sum_numbers.c
)

