# Add the headers install target
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/arrowhead
  DESTINATION include
  COMPONENT headers
  PATTERN "*.in" EXCLUDE
  PATTERN "doc.txt" EXCLUDE)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/nlohmann
  DESTINATION include
  COMPONENT headers
  PATTERN "config.h.in" EXCLUDE
  PATTERN "doc.txt" EXCLUDE)

# Also, add the auto-generated header files
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/arrowhead
  DESTINATION include
  COMPONENT headers)
