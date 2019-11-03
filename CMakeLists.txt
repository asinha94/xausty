cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(ENABLE_ASAN "ENABLE_ASAN" true)
option(ENABLE_UBSAN "ENABLE_UBSAN" true)

if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif(ENABLE_ASAN)

if(ENABLE_UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif(ENABLE_UBSAN)

include_directories(include)

add_executable(xausty src/main.cc)
