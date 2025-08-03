cmake_minimum_required(VERSION 3.15)
project(Cratch C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(cratch
        src/main.c
        src/zip.c
        src/zip.h
)

# make sure the compiler can find include/miniz/miniz.h
target_include_directories(cratch PRIVATE include)

# Build miniz as a static lib and link it
add_library(miniz STATIC miniz/miniz.c)
target_include_directories(miniz PUBLIC include/miniz)  # if miniz.h is at include/miniz.h

target_link_libraries(cratch PRIVATE miniz)

target_compile_options(cratch PRIVATE -Wall -Wextra -Wpedantic)
