#
# include paths
#
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main ${PROJECT_NAME})

if(ARROWHEAD_USE_LOG4CPLUS)
  include_directories(${LOG4CPLUS_INCLUDE_DIRS})
endif()

# XML tests
if(ARROWHEAD_USE_PUGIXML)
  add_executable(test_xml xml/test_parse.cpp)
  add_test(XML test_xml)
  add_dependencies(test_xml version)
  target_link_libraries(test_xml test_main)
  target_link_libraries(test_xml ${PUGIXML_LIBRARY})
  target_link_libraries(test_xml ${PROJECT_NAME})
endif()

# HTTP tests
if(ARROWHEAD_USE_LIBCURL)
  add_executable(test_serviceregistry core_services/test_servicelist.cpp)
  add_test(ServiceRegistry test_serviceregistry)
  add_dependencies(test_serviceregistry version)
  target_link_libraries(test_serviceregistry test_main)
  target_link_libraries(test_serviceregistry ${LIBCURL_LIBRARY})
  target_link_libraries(test_serviceregistry ${PROJECT_NAME})
endif()

# JSON tests
if(ARROWHEAD_USE_JSON)
  add_executable(test_json json/test_parse.cpp)
  add_test(JSON test_json)
  add_dependencies(test_json version)
  target_link_libraries(test_json test_main)
  target_link_libraries(test_json ${PROJECT_NAME})
endif()

