cmake_minimum_required(VERSION 3.13)
project(LoF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -fopt-info-vec")
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(LoF main.cpp)