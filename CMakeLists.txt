cmake_minimum_required(VERSION 3.21)
project(abgx360)

set(CMAKE_C_STANDARD 23)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(src)

# Add executable
add_executable(abgx360
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
target_link_libraries(abgx360 m)
target_link_libraries(abgx360 CURL::libcurl)
target_link_libraries(abgx360 ZLIB::ZLIB)