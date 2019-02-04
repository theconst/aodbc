cmake_minimum_required(VERSION 3.2)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project("cacheodbc")

macro(fetch_submodule MODULE_NAME)
    message("Fetching submodule: ${MODULE_NAME}")
    execute_process(
        COMMAND git submodule update --init -- ${MODULE_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

########## Windows flags ######

# Suppress warnings for MSVC
if (MSVC)
    # See also https://github.com/nodejs/node/pull/15570
    add_compile_options(/wd4251) # disabled by node
    add_compile_options(/wd4275) # disabled by node
endif()

################################

####### CMake policies #########

# Enable BOOST_ROOT enviromental variable (CMP0074)
cmake_policy(VERSION 3.12)

################################

###### Boost dependency ######
message("Finding boost dependencies")
find_package(Boost 1.62 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

################################

###### Nanodbc dependency ######

fetch_submodule(nanodbc)

option(NANODBC_DISABLE_TESTS "Build tests for nanodbc" ON)
option(NANODBC_DISABLE_EXAMPLES "Build samples for nanodbc" ON)


option(NANODBC_ENABLE_UNICODE "Enable unicode. Unset this option if you're using unixODBC" ON)
if (NANODBC_ENABLE_UNICODE)
    # Set flags globally
    if (UNIX OR APPLE)
        find_program(IODBC_CONFIG iodbc-config
            $ENV{ODBC_PATH}/bin /usr/bin /usr/local/bin ~/bin PATHS)
        if (NOT IODBC_CONFIG)
            message(FATAL_ERROR "Only iODBC driver is supported on UNIX-like systems")
        else()
            execute_process(COMMAND ${IODBC_CONFIG} --cflags
                OUTPUT_VARIABLE ODBC_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
        endif()


        add_definitions(-DNANODBC_USE_IODBC_WIDE_STRINGS)
    endif()
    add_definitions(-DNANODBC_ENABLE_UNICODE)
endif()

add_subdirectory(nanodbc)

# Build as static lib, this will allow uniformity across platforms
# Library binary is still below 1MB, which far than OK for js world

################################

######## nc.js options #########

# This is a workaround for the database driver http://www.intersystems.com/ru/our-products/cache/cache-overview/
# Enabled by default
option(NC_FREE_STMT_ONLY "Call only SQLFreeStmt(SQL_CLOSE) before result fetch. Do not call SQLCloseCursor after result fetch")
if (NC_FREE_STMT_ONLY)
    add_definitions(-DFREE_STMT_WORKAROUND_OFF)
endif()
message(STATUS "cache-odbc: Call SQLFreeStmt - ${NC_FREE_STMT_ONLY}")

###### Build shared library ####

string(REPLACE "." ";" NODE_RUNTIMEVERSION_LIST ${NODE_RUNTIMEVERSION})
list(GET NODE_RUNTIMEVERSION_LIST 0 NODE_MAJOR_VERSION)

set(FULL_PROJECT_NAME "${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${NODE_ARCH}-v${NODE_MAJOR_VERSION}")
message("Set library name: ${FULL_PROJECT_NAME}")

file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")

add_library(${FULL_PROJECT_NAME} SHARED ${SOURCE_FILES})

if (ODBC_CFLAGS)
    set_target_properties(${FULL_PROJECT_NAME} 
        PROPERTIES PREFIX ""  SUFFIX ".node" COMPILE_FLAGS ${ODBC_CFLAGS})
else()
    set_target_properties(${FULL_PROJECT_NAME} PROPERTIES PREFIX ""  SUFFIX ".node")
endif()
target_include_directories(${FULL_PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${FULL_PROJECT_NAME} PUBLIC ${CMAKE_JS_LIB} PRIVATE nanodbc)

################################