# xausty

HTTP Server built from scratch from Linux. The name is a play on words I came up with in the middle of the night.
nginx (Engine X) == xausty (Exhaust y).
This is mostly my attempt at learning how to use epoll, fork, mmap, aio etc.. to build 'scalable' networking software.
C++17 is used for classes, namespacing, copy ellision, operator new etc...

## Requirements

- CMake > 3.8. You might need to build a newer release from sources here: https://cmake.org/download/

- C++17 Compiler. GCC preffered but Clang should work

- Linux Kernel version > 4

## TODO (Immediate):

- Refactor main loop into epoll event loop to make it prettier and more manageable

- Change epoll back to level triggered and disable write when necessary

- Create HTTP abstraction/handlers