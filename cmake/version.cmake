ADD_CUSTOM_TARGET(
    version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/cmake/project_version.h.in
                     -D DST=${CMAKE_BINARY_DIR}/include/project_version.h
                     -P ${CMAKE_SOURCE_DIR}/cmake/generate-VCS_VERSION.cmake
)
include_directories(${CMAKE_BINARY_DIR}/include)

ADD_CUSTOM_TARGET(
    doxyfile_version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/doc/PROJECT_NUMBER.doxy.in
                     -D DST=${CMAKE_BINARY_DIR}/doc/PROJECT_NUMBER.doxy
                     -P ${CMAKE_SOURCE_DIR}/cmake/generate-VCS_VERSION.cmake
)
