# xausty

HTTP Server built from scratch from Linux. The name is a play on words I came up with in the middle of the night.
nginx (Engine X) == xausty (Exhaust y).
This is mostly my attempt at learning how to use epoll, fork, mmap, aio etc.. to build 'scalable' networking software.
C++17 is used for classes, namespacing, copy ellision, operator new etc...

## Requirements

- CMake > 3.8
- C++17 Compiler (GCC)
- Linux Kernel version > 4 should work

To install cmake you may need to install a newer version from the official releases website at https://cmake.org/download/