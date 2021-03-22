cmake_minimum_required(VERSION 3.7)
project(tests LANGUAGES C)

set(CMAKE_C_STANDARD 99)

#add_executable(converter /src/Converter/converter.c)
add_executable(comparer /src/Comparer/comparer.c /src/Comparer/comparer_lib.c)
