set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/examples)
if(ARROWHEAD_USE_LIBCOAP)
  # Build the ah-coap-server example
  find_package(Boost 1.40 COMPONENTS program_options system REQUIRED)
  #~ set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)

  set(COAP_SERVER_SRC_FILES
      ah-coap-server/coap-server.cpp
      )
  add_executable(ah-coap-server ${COAP_SERVER_SRC_FILES})
  add_dependencies(ah-coap-server version)
  # Link to libarrowhead
  target_link_libraries(ah-coap-server ${PROJECT_NAME})
  # and Boost (for Boost::asio)
  target_link_libraries(ah-coap-server ${COAP_LIBRARIES})
  target_link_libraries(ah-coap-server ${Boost_LIBRARIES})
  target_link_libraries(ah-coap-server Threads::Threads)
  install(TARGETS ah-coap-server
    RUNTIME DESTINATION bin
    COMPONENT examples)
else()
  message("ah-coap-server requires ARROWHEAD_USE_LIBCOAP ON")
endif()
