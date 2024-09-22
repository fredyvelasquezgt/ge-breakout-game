cmake_minimum_required(VERSION 3.28)
project(tarea1)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(tarea1 main.cpp)
target_link_libraries(tarea1 ${SDL2_LIBRARIES})


