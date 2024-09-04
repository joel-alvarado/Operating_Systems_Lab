cmake_minimum_required(VERSION 3.5.0)
project(exercise_1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(greetings 
    src/greetings.c
    src/greetlib.c
)

add_executable(fork_maker
    src/fork_maker.c
    src/child_creator.c
)

