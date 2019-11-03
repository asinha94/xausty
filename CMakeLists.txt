cmake_minimum_required(VERSION 3.5.1)
project(xausty)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(ENABLE_ASAN "ENABLE_ASAN" false)
option(ENABLE_UBSAN "ENABLE_UBSAN" false)

# Add the compile/link time flags
if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif(ENABLE_ASAN)

if(ENABLE_UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif(ENABLE_UBSAN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -Werror -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wcast-qual -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -g")

include_directories(include)
file(GLOB_RECURSE xausty_srcs "src/*.cc")
add_executable(xausty ${xausty_srcs})
