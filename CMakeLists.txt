cmake_minimum_required(VERSION 3.22)
project(thecube)

set(CMAKE_C_STANDARD 17)

add_executable(thecube main.c vm.c)

# Testing
add_executable(tester test.c vm.c)
enable_testing()

add_test(NAME mytest COMMAND tester bytes_to_int)
