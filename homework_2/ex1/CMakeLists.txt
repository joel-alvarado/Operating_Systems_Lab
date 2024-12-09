cmake_minimum_required(VERSION 3.5.0)
project(main VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -std=c99")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -s -std=c99")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG -std=c99")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -s -std=c99")


include_directories(include)

add_executable(${CMAKE_PROJECT_NAME} 
    src/main2.c
)