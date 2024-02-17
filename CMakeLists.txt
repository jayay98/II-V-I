cmake_minimum_required(VERSION 3.27)

if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
# If no deployment target has been set default to the minimum supported
# OS version (this has to be set before the first project() call)
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "iOS Deployment Target")
else()
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "macOS Deployment Target")
endif()
endif()

project(iivi VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_PROJECTS OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/vendors/stk/cmake/")
set(wxBUILD_SHARED OFF)
set(wxBUILD_COMPATIBILITY "3.2")

add_definitions(-D__MACOSX_CORE__)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_subdirectory(vendors/stk)
include_directories(vendors/stk/include)

add_subdirectory(vendors/wx)
include_directories(vendors/wx/include)

set(SRC_FILES src/main.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} wx::net wx::core wx::base ${CMAKE_THREAD_vendors_INIT} stk)