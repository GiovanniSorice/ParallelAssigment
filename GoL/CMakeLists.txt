cmake_minimum_required(VERSION 3.11)
project(GoL VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenMP REQUIRED)


set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -fsanitize=thread -fstack-check -fstack-protector-strong -fno-omit-frame-pointer -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -pthread")
set(CMAKE_CXX_FLAGS -pthread)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(GoL main.cpp GoLSeq.h GoLThread.h GoLOpenMP.h GoLPool.h taskQueue.h)

target_link_libraries(GoL
        PUBLIC
        OpenMP::OpenMP_CXX
        )