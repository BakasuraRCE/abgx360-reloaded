cmake_minimum_required(VERSION 3.21)
project(abgx360)

set(CMAKE_C_STANDARD 23)

include_directories(src)

# Add executable
add_executable(abgx360
        src/abgx360.c
        src/rijndael-alg-fst.c
        src/rijndael-alg-fst.h
        src/sha1.c
        src/sha1.h)