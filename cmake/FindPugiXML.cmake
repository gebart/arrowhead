# Find the PugiXML XML parsing library.
#
# Sets the usual variables expected for find_package scripts:
#
# PUGIXML_INCLUDE_DIR - header location
# PUGIXML_LIBRARIES - library to link against
# PUGIXML_FOUND - true if pugixml was found.

find_path (PUGIXML_INCLUDE_DIR
           NAMES pugixml.hpp
           PATH_SUFFIXES pugixml
)
mark_as_advanced (PUGIXML_INCLUDE_DIR)

find_library (PUGIXML_LIBRARY
              NAMES pugixml)
mark_as_advanced (PUGIXML_LIBRARY)

if(PUGIXML_INCLUDE_DIR AND EXISTS "${PUGIXML_INCLUDE_DIR}/pugixml.hpp")
    file(STRINGS "${PUGIXML_INCLUDE_DIR}/pugixml.hpp" PUGIXML_VERSION_LINE REGEX "^[ \t]*#[ \t]*define[ \t]+PUGIXML_VERSION[ \t]+")
    string(REGEX REPLACE "^[ \t]*#[ \t]*define[ \t]+PUGIXML_VERSION[ \t]+([0-9]+)$" "\\1" PUGIXML_VERSION_NUM "${PUGIXML_VERSION_LINE}")
    string(REGEX REPLACE "^([0-9]+)([0-9][0-9])$" "\\1.\\2" PUGIXML_VERSION_STRING "${PUGIXML_VERSION_NUM}")
    unset(PUGIXML_VERSION_LINE)
    unset(PUGIXML_VERSION_NUM)
endif()

# Support the REQUIRED and QUIET arguments, and set PUGIXML_FOUND if found.
include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (PugiXML
                                   REQUIRED_VARS PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR
                                   VERSION_VAR PUGIXML_VERSION_STRING
)

if (PUGIXML_FOUND)
    set (PUGIXML_LIBRARIES ${PUGIXML_LIBRARY})
    set (PUGIXML_INCLUDE_DIRS ${PUGIXML_INCLUDE_DIR})
else ()
    message (STATUS "No PugiXML found")
endif()

