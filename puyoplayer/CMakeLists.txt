cmake_minimum_required(VERSION 3.6)
project(puyoplayer)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -std=c11 -lcurl")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

set(SOURCE_FILES src/main.c src/server.c src/server.h src/game.c src/game.h)
add_executable(puyoplayer ${SOURCE_FILES})