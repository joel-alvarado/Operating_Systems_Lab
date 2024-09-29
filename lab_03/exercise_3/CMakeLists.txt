cmake_minimum_required(VERSION 3.5.0)
project(exercise_3 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(exercise_3
    src/thread_routines.c
    src/main.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC m)

