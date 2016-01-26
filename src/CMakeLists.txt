#
# dependencies
#

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# Build libarrowheadcpp
set(LIB_SRC_FILES
    core_services/serviceregistry.cpp
    content/xml.cpp
    logging/logging.cpp
    transport/http.cpp
    )
add_library(${PROJECT_NAME} ${LIB_SRC_FILES})
add_dependencies(${PROJECT_NAME} version)

if(ARROWHEAD_USE_LIBCURL)
  target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
endif()

if(ARROWHEAD_USE_PUGIXML)
  target_link_libraries(${PROJECT_NAME} ${PUGIXML_LIBRARY})
endif()

if(ARROWHEAD_USE_LOG4CPLUS)
  target_link_libraries(${PROJECT_NAME} ${LOG4CPLUS_LIBRARIES})
endif()


# Build the ahq (Arrowhead Query) command line utility
set(AHQ_SRC_FILES
    cli/ahq.cpp
    )
add_executable(ahq ${AHQ_SRC_FILES})
add_dependencies(ahq version)
# Link to libarrowheadcpp
target_link_libraries(ahq ${PROJECT_NAME})
# and Boost (for Boost::program_options)
target_link_libraries(ahq ${Boost_LIBRARIES})

#
# installation & bundling
#
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
