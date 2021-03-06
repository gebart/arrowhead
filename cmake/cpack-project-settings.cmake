set(CPACK_PACKAGE_NAME "libarrowhead")
set(CPACK_PACKAGE_VENDOR "Fotonic")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Arrowhead C++ interface")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Arrowhead")

# Available CPack component choices
set(CPACK_COMPONENTS_ALL applications libraries headers)

set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Tools (ahq)")
set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Example applications (ah-coap-server)")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Shared libraries (libarrowhead)")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")
# Let the C++ headers component depend on the library component
set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)
# Let the applications component depend on the library component
set(CPACK_COMPONENT_APPLICATIONS_DEPENDS libraries)
set(CPACK_COMPONENT_EXAMPLES_DEPENDS libraries)

set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_COMPONENT_EXAMPLES_GROUP "Runtime")

set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
   "Components for developers")
