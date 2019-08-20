cmake_minimum_required(VERSION 3.13)

project(
  CostModel VERSION 1.0
  DESCRIPTION "A cost model for memory accesses"
  LANGUAGES CXX
)

set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

find_package(LEMON REQUIRED)

add_subdirectory(src)
