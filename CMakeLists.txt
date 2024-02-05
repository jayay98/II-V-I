cmake_minimum_required(VERSION 3.27)
project(iivi VERSION 1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/libs/stk/cmake/")

set(COMPILE_PROJECTS OFF)
add_subdirectory(libs/stk)
include_directories(libs/stk/include)

add_executable(main src/main.cpp)
target_link_libraries(main stk)