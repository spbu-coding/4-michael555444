cmake_minimum_required(VERSION 3.7)
project(tests LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(converter /src/Converter/main.c /src/Converter/includes.h /src/Converter/SelfWrittenConverter/my_converter.c /src/Converter/SelfWrittenConverter/my_converter.h /src/Converter/ThirdPartyWrittenConverter/qdbmp.c /src/Converter/ThirdPartyWrittenConverter/qdbmp.h)
add_executable(comparer /src/Comparer/comparer.c /src/Comparer/comparer.h)
