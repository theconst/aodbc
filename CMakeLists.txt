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

###### Boost dependency ######
message("Fetching boost dependencies")
fetch_submodule(boost/variant)
fetch_submodule(boost/optional)
include_directories(boost)

################################

###### Nanodbc dependency ######

fetch_submodule(nanodbc)

# Build as static lib, this will allow uniformity across platforms
# Library binary is still below 1MB, which far than OK for js world
option(NANODBC_ENABLE_UNICODE OFF "Enable unicode for ODBC. This depends on ODBC driver you are working with")

option(NANODBC_DISABLE_TESTS "Build tests for nanodbc" ON)
option(NANODBC_DISABLE_EXAMPLES "Build samples for nanodbc" ON)
add_subdirectory(nanodbc)

################################

######## ns.js options #########

# This is a workaround for the database driver http://www.intersystems.com/ru/our-products/cache/cache-overview/
# Enabled by default
option(NC_FREE_STMT_ONLY "Call only SQLFreeStmt(SQL_CLOSE) before result fetch. Do not call SQLCloseCursor after result fetch")
if (NC_FREE_STMT_ONLY)
    add_definitions(-DFREE_STMT_WORKAROUND_OFF)
endif()
message(STATUS "cache-odbc: Call SQLFreeStmt is - ${NC_FREE_STMT_ONLY}")

###### Build shared library ####

file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} PRIVATE nanodbc)

################################