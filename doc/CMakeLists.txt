# add a target to generate API documentation with Doxygen
find_package(Doxygen)

OPTION(DOXYGEN_CLANG_ASSISTED_PARSING "Use the Doxygen Clang/LLVM parser when generating documentation" OFF)
mark_as_advanced(DOXYGEN_CLANG_ASSISTED_PARSING)

if(DOXYGEN_CLANG_ASSISTED_PARSING)
    set(CLANG_ASSISTED_PARSING YES)
else()
    set(CLANG_ASSISTED_PARSING NO)
endif(DOXYGEN_CLANG_ASSISTED_PARSING)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_dependencies(doc doxyfile_version)
endif(DOXYGEN_FOUND)
