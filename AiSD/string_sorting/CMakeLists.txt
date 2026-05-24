cmake_minimum_required(VERSION 3.10)
project(StringSortingBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

include_directories(include)

add_executable(benchmark 
    src/main.cpp 
    src/StringGenerator.cpp 
    src/StringSortTester.cpp
)
