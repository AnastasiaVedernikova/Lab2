cmake_minimum_required(VERSION 3.6)
project(Lab)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(Lab ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( Lab pthread)