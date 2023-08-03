cmake_minimum_required(VERSION 3.10.0)
set(TML_VERSION 0.0.1)
project(tml LANGUAGES C VERSION ${TML_VERSION})

option(DEBUG_MODE "Compile in debug mode" OFF)
option(COMPILE_TESTS "Compile tests" ON)

if (DEBUG_MODE)
  message("Compiling in debug mode...")
  set(CMAKE_C_FLAGS "-O0 -g -Wall -Wextra -Wpedantic -fsanitize=address")
else()
  message("Compiling in release mode...")
  set(CMAKE_C_FLAGS "-O2 -Wall")
endif()

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(src)

if (COMPILE_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()
