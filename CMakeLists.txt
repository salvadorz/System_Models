#******************************************************************************
#*Copyright (C) 2023 by Salvador Z                                            *
#*                                                                            *
#*****************************************************************************/
#*
#*@author Salvador Z
#*@brief CMakeLists file to create a CMake project
#*
cmake_minimum_required(VERSION 3.0.0)
project(System_Models
  VERSION 0.1.0
  DESCRIPTION "Project to model Cross-Functional Systems"
  LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

### Build Options ###
option(ENABLE_COVERAGE "Enable coverage reporting"              OFF)

### General Configuration ###

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

### Compiler Flags Configuration ###

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

##########################
# Enable Static Analysis #
##########################
#include(analysis/code-format)


### General Project Structure ###

if (UNIX)
    set(SystemC_DIR "/opt/systemC")
endif (UNIX)

find_package(SystemCLanguage 2.3.4 REQUIRED)

if(SystemCLanguage_FOUND)
  include_directories(${SystemC_DIR}/include)
  # Create an output dir in CMAKE_BINARY_DIR
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
  message(FATAL_ERROR "SystemC not found!!")
endif()

# Use this directory to test and emulate a filesystem env
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/artifacts)

### Project Source Tree ###
add_subdirectory("src")