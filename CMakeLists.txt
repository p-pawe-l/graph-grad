cmake_minimum_required(VERSION 3.16)

project(mygrad LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Library with the autograd core
add_library(mygrad_lib
  src/graph.cpp
)
target_include_directories(mygrad_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(mygrad_lib PRIVATE -Wall -Wextra -Wpedantic)

# Main executable
add_executable(mygrad main.cpp)
target_link_libraries(mygrad PRIVATE mygrad_lib)
target_compile_options(mygrad PRIVATE -Wall -Wextra -Wpedantic)
