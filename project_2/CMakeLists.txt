cmake_minimum_required(VERSION 3.5.0)
project(imu_driver VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s")

include_directories(include)

add_executable(${CMAKE_PROJECT_NAME} 
    src/start_driver.c
    src/driver.c
)

add_executable(supervisor
    src/start_supervisor.c
    src/supervisor.c
)

