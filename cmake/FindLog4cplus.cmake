# Find the Log4cplus XML parsing library.
#
# Sets the usual variables expected for find_package scripts:
#
# LOG4CPLUS_INCLUDE_DIRS - header location
# LOG4CPLUS_LIBRARIES - library to link against
# LOG4CPLUS_FOUND - true if Log4cplus was found.

find_path (LOG4CPLUS_INCLUDE_DIR
           NAMES log4cplus/version.h
           PATH_SUFFIXES log4cplus
)
mark_as_advanced (LOG4CPLUS_INCLUDE_DIR)

find_library (LOG4CPLUS_LIBRARY NAMES
              log4cplus
)
mark_as_advanced (LOG4CPLUS_LIBRARY)

if(LOG4CPLUS_INCLUDE_DIR AND EXISTS "${LOG4CPLUS_INCLUDE_DIR}/log4cplus/version.h")
    file(STRINGS "${LOG4CPLUS_INCLUDE_DIR}/log4cplus/version.h" LOG4CPLUS_VERSION_LINE REGEX "^[ \t]*#[ \t]*define[ \t]+LOG4CPLUS_VERSION[ \t]+")
    string(REGEX REPLACE "^[ \t]*#[ \t]*define[ \t]+LOG4CPLUS_VERSION[ \t]+.*[ \t]*([0-9]+)[ \t]*,[ \t]*([0-9]+)[ \t]*,[ \t]*([0-9]+)[ \t]*.*$" "\\1.\\2.\\3" LOG4CPLUS_VERSION_STRING "${LOG4CPLUS_VERSION_LINE}")
    unset(LOG4CPLUS_VERSION_LINE)
endif()

include(FindPackageHandleStandardArgs)

# Support the REQUIRED and QUIET arguments, and set LOG4CPLUS_FOUND if found.
find_package_handle_standard_args(Log4cplus
                                  REQUIRED_VARS LOG4CPLUS_LIBRARY LOG4CPLUS_INCLUDE_DIR
                                  VERSION_VAR LOG4CPLUS_VERSION_STRING
)

if (LOG4CPLUS_FOUND)
    set (LOG4CPLUS_LIBRARIES ${LOG4CPLUS_LIBRARY})
    set (LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIR})
else ()
    message (STATUS "No Log4cplus found")
endif()
