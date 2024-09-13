cmake_minimum_required(VERSION 3.5.0)
project(exercise_2 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(exercise_2
    src/main.c
)

add_executable(spawn_counters
    src/spawn_counters.c
)

add_executable(counter_handler 
    src/counter_handler.c
)

add_executable(infinite_counter 
    src/infinte_counter.c
)

