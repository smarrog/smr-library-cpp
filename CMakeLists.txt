cmake_minimum_required(VERSION 3.14)
project(smr)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_subdirectory(src)

add_executable(smr ${SOURCE_FILES} main.cpp)