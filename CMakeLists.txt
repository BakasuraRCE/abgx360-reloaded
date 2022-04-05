cmake_minimum_required(VERSION 3.21)
set(target abgx360)
project(${target})

set(CMAKE_CXX_STANDARD 23)

find_package(CURL CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(src)

# Add executable
add_executable(${target}
        src/global.h
        src/utils.cpp
        src/utils.h
        src/abgx360.c
        src/rijndael-alg-fst.c
        src/rijndael-alg-fst.h
        src/sha1.c
        src/sha1.h
        src/mspack/lzx.h
        src/mspack/lzxd.c
        src/mspack/mspack.h
        src/mspack/system.c
        src/mspack/system.h)
target_link_libraries(${target} PRIVATE m)
target_link_libraries(${target} PRIVATE CURL::libcurl)
target_link_libraries(${target} PRIVATE ZLIB::ZLIB)