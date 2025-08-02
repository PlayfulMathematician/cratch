cmake_minimum_required(VERSION 3.31)
project(Cratch C)

set(CMAKE_C_STANDARD 11)

set(SOURCES src/main.c)

add_executable(cratch ${SOURCES})

target_include_directories(cratch PRIVATE include)

target_compile_options(cratch PRIVATE -Wall -Wextra -Wpedantic)
