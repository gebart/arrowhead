cmake_minimum_required(VERSION 3.1)
project(arrowhead)

# Setup CMAKE
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(version)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# Put libraries in lib instead of build/src/...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# We use standard C++11 throughout
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Setting CXX_EXTENSIONS to ON makes CMake add -std=gnu++11 instead of -std=c++11,
# we aim for cross-platform and standards compliant code so we disable it.
set(CMAKE_CXX_EXTENSIONS OFF)

#
# compiler & linker configuration
#
message(STATUS "Compiler: " ${CMAKE_CXX_COMPILER_ID})
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # GCC
  add_compile_options(-Wall -Wextra -pedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # Clang or AppleClang
  add_compile_options(-Wall -Wextra -pedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # TODO: Visual Studio
endif()


#
# Project settings
#
option(BUILD_SHARED_LIBS "Build shared libraries." ON)

option(ARROWHEAD_USE_LOG4CPLUS "Build library with logging using log4cplus" ON)
if(ARROWHEAD_USE_LOG4CPLUS)
  find_package(Log4cplus REQUIRED)
endif()

option(ARROWHEAD_USE_PUGIXML "Build library with XML format support using PugiXML" ON)
if(ARROWHEAD_USE_PUGIXML)
  find_package(PugiXML REQUIRED)
endif()

option(ARROWHEAD_USE_LIBCURL "Build library with HTTP support using libcurl" ON)
if(ARROWHEAD_USE_LIBCURL)
  find_package(CURL REQUIRED)
endif()

option(ARROWHEAD_USE_LIBCOAP "Build library with CoAP support using libcoap" ON)
if(ARROWHEAD_USE_LIBCOAP)
  find_package(CoAP REQUIRED)
endif()

configure_file(${CMAKE_SOURCE_DIR}/include/arrowhead/config.h.in ${CMAKE_BINARY_DIR}/include/arrowhead/config.h)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# Enable unit testing via `make test`
enable_testing()

#
# include paths
#
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIR})
if(ARROWHEAD_USE_LIBCURL)
  include_directories(${CURL_INCLUDE_DIRS})
endif()

if(ARROWHEAD_USE_PUGIXML)
  include_directories(${PUGIXML_INCLUDE_DIR})
endif()

if(ARROWHEAD_USE_LOG4CPLUS)
  include_directories(${LOG4CPLUS_INCLUDE_DIRS})
endif()

add_subdirectory(tests)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_subdirectory(src)
add_subdirectory(doc)
