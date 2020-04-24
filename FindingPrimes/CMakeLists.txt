cmake_minimum_required(VERSION 3.11)
project(FindingPrimes VERSION 1.0.0 LANGUAGES CXX)

# find package needed by the project
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -fsanitize=thread -fstack-check -fstack-protector-strong -fno-omit-frame-pointer -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -pthread")

# creating the target FindingPrimes
add_executable(FindingPrimes main.cpp)

add_library(master_worker master_worker.h)
add_library(parallel_for_exec parallel_for_exec.h)

set_target_properties(master_worker PROPERTIES
        CXX_STANDARD 17
        LINKER_LANGUAGE CXX)

set_target_properties(parallel_for_exec PROPERTIES
        CXX_STANDARD 17
        LINKER_LANGUAGE CXX)

# set compiler
target_compile_features(FindingPrimes PUBLIC cxx_std_17)

# link libraries
target_link_libraries(FindingPrimes
        OpenMP::OpenMP_CXX
        Threads::Threads
        master_worker
        )
