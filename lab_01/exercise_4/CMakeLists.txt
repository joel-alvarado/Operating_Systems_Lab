cmake_minimum_required(VERSION 3.5.0)
project(exercise_4 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(exercise_4 
    src/main.c
    src/str_handler.c
)

