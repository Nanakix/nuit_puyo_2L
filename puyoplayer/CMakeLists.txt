cmake_minimum_required(VERSION 3.6)
project(puyoplayer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c11")

set(SOURCE_FILES main.c)
add_executable(puyoplayer ${SOURCE_FILES})