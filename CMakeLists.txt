cmake_minimum_required(VERSION 3.2)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Name of the project (will be the name of the plugin)
project(nc)

###### Boost dependency ######

# Include BoostLib module
file(GLOB_RECURSE boostlib_cmake_path "${CMAKE_CURRENT_SOURCE_DIR}/node_modules" "BoostLib.cmake")
list(GET boostlib_cmake_path 0 boostlib_cmake_path)
get_filename_component(boostlib_cmake_path "${boostlib_cmake_path}" DIRECTORY)
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${boostlib_cmake_path}")
include(BoostLib)

# Locate/Download Boost (semver)
require_boost_libs(">= 1.62.0" "variant" "optional")
include_directories(${Boost_INCLUDE_DIRS})

###############################

###### Nanodbc dependency ######

# Build as static lib, this will allow uniformity across platforms
# Executable is still below 1MB, which far than OK for js world
option(BUILD_SHARED_LIBS "Build nanodbc as shared library (UNIX only)")
option(NANODBC_ENABLE_UNICODE OFF "Enable unicode for ODBC. This depends on ODBC driver you are working with")

option(NANODBC_DISABLE_TESTS "Build tests for nanodbc" ON)
option(NANODBC_DISABLE_EXAMPLES "Build samples for nanodbc" ON)
add_subdirectory(nanodbc)

################################

# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} nanodbc)