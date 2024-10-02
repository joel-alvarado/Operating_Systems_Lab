cmake_minimum_required(VERSION 3.5.0)
project(exam_practice VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(fork
    src/fork.c
)

add_executable(execvp
    src/execvp.c
)

add_executable(signals
    src/signals.c
)

add_executable(waits
    src/waits.c
)

add_executable(shm
    src/shm_semaphores.c
)

add_executable(message_queues
    src/message_queues.c
)

add_executable(threads
    src/threads.c
)