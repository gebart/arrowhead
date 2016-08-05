#
# dependencies
#

# Build libarrowheadcpp
set(LIB_SRC_FILES
    core_services/serviceregistry.cpp
    content/xml.cpp
    content/json.cpp
    logging/logging.cpp
    transport/http.cpp
    )
add_library(${PROJECT_NAME} ${LIB_SRC_FILES})
add_dependencies(${PROJECT_NAME} version)

if(ARROWHEAD_USE_LIBCURL)
  target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
endif()

if(ARROWHEAD_USE_PUGIXML)
  target_link_libraries(${PROJECT_NAME} ${PUGIXML_LIBRARIES})
endif()

if(ARROWHEAD_USE_LOG4CPLUS)
  target_link_libraries(${PROJECT_NAME} ${LOG4CPLUS_LIBRARIES})
endif()

if(ARROWHEAD_USE_LIBCOAP)
# Build the coap-server experiment
find_package(Boost 1.40 COMPONENTS program_options system REQUIRED)

set(COAP_SERVER_SRC_FILES
    server/coap-server.cpp
    )
add_executable(coap-server ${COAP_SERVER_SRC_FILES})
add_dependencies(coap-server version)
# Link to libarrowheadcpp
target_link_libraries(coap-server ${PROJECT_NAME})
# and Boost (for Boost::asio)
target_link_libraries(coap-server ${COAP_LIBRARIES})
endif()

if(ARROWHEAD_USE_PUGIXML AND ARROWHEAD_USE_LIBCURL)
# Build the ahq (Arrowhead Query) command line utility
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

set(AHQ_SRC_FILES
    cli/ahq.cpp
    )
add_executable(ahq ${AHQ_SRC_FILES})
add_dependencies(ahq version)
# Link to libarrowheadcpp
target_link_libraries(ahq ${PROJECT_NAME})
# and Boost (for Boost::program_options)
target_link_libraries(ahq ${Boost_LIBRARIES})
else()
  message("ahq requires both libcurl and pugixml to be enabled")
endif()

#
# installation & bundling
#
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_BINARY_DIR})
