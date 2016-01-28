CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

find_package(Git)
execute_process(
     COMMAND ${GIT_EXECUTABLE} describe --exact-match
     OUTPUT_VARIABLE VCS_VERSION
     ERROR_VARIABLE _git_error_msg
     OUTPUT_STRIP_TRAILING_WHITESPACE
)
if("${VCS_VERSION}" STREQUAL "")
     execute_process(
          COMMAND ${GIT_EXECUTABLE} log --pretty=format:%cd-g%h --date=short --abbrev-commit -1
          OUTPUT_VARIABLE VCS_VERSION
          ERROR_VARIABLE _git_error_msg
          OUTPUT_STRIP_TRAILING_WHITESPACE
     )
endif()
message(STATUS "VCS version: " ${VCS_VERSION})
configure_file(${SRC} ${DST} @ONLY)
