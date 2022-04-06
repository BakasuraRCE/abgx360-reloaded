cmake_minimum_required(VERSION 3.21)
set(target abgx360)
project(${target})

set(CMAKE_CXX_STANDARD 23)

if (UNIX)
    find_package(CURL REQUIRED)
else ()
    find_package(CURL CONFIG REQUIRED)
endif ()
find_package(ZLIB REQUIRED)

include_directories(src)

# Add executable
add_executable(${target}
        src/utils.h
        src/utils.cpp
        src/abgx360.h
        src/variables.h
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
if (UNIX)
    target_link_libraries(${target} PRIVATE m)
endif ()
target_link_libraries(${target} PRIVATE CURL::libcurl)
target_link_libraries(${target} PRIVATE ZLIB::ZLIB)