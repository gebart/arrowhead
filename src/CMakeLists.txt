#
# dependencies
#

# Build libarrowhead
set(LIB_SRC_FILES
    core_services/serviceregistry.cpp
    content/xml.cpp
    content/json.cpp
    logging/logging.cpp
    transport/http.cpp
    transport/coap.cpp
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
  target_link_libraries(${PROJECT_NAME} ${COAP_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT libraries)

if(ARROWHEAD_BUILD_TOOLS)
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
  if(ARROWHEAD_USE_JSON AND ARROWHEAD_USE_LIBCURL)
    # Build the ahq (Arrowhead Query) command line utility
    find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

    set(AHQ_SRC_FILES
      cli/ahq.cpp
      )
    add_executable(ahq ${AHQ_SRC_FILES})
    add_dependencies(ahq version)
    # Link to libarrowhead
    target_link_libraries(ahq ${PROJECT_NAME})
    # and Boost (for Boost::program_options)
    target_link_libraries(ahq ${Boost_LIBRARIES})
    install(TARGETS ahq
      RUNTIME DESTINATION bin
      COMPONENT applications)
  else()
    message("ahq requires both libcurl and json to be enabled")
  endif()
endif()
