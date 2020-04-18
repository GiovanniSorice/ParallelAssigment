cmake_minimum_required(VERSION 3.11)
project(LoF VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenMP REQUIRED)


set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -fsanitize=thread -fstack-check -fstack-protector-strong -fno-omit-frame-pointer -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -fopt-info-vec-missed")
set(CMAKE_CXX_FLAGS -pthread)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(LoF main.cpp GoLSeq.h GoLThread.h)

target_link_libraries(LoF
        PUBLIC
        OpenMP::OpenMP_CXX
        )