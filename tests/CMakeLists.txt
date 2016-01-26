#
# include paths
#
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

if(ARROWHEAD_USE_LOG4CPLUS)
  include_directories(${LOG4CPLUS_INCLUDE_DIRS})
endif()

# XML tests
if(ARROWHEAD_USE_PUGIXML)
  add_executable(test_xml test_main.cpp xml/test_parse.cpp)
  add_test(XML test_xml)
  add_dependencies(test_xml version)
  target_link_libraries(test_xml ${PUGIXML_LIBRARY})
  target_link_libraries(test_xml ${PROJECT_NAME})
  if(ARROWHEAD_USE_LOG4CPLUS)
    target_link_libraries(test_xml ${LOG4CPLUS_LIBRARIES})
  endif()
endif()

# HTTP tests
if(ARROWHEAD_USE_LIBCURL)
  add_executable(test_serviceregistry test_main.cpp core_services/test_servicelist.cpp)
  add_test(ServiceRegistry test_serviceregistry)
  add_dependencies(test_serviceregistry version)
  target_link_libraries(test_serviceregistry ${LIBCURL_LIBRARY})
  target_link_libraries(test_serviceregistry ${PROJECT_NAME})
  if(ARROWHEAD_USE_LOG4CPLUS)
    target_link_libraries(test_serviceregistry ${LOG4CPLUS_LIBRARIES})
  endif()
endif()

