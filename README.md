# Arrowhead C++ library

This is a C++ library for interfacing with Arrowhead services.

This project is very much a work in progress and the API is still subject to
major changes on a daily basis. The library is not ready for mainstream usage.

The library relies on C++11 features, such as range-based for and auto variables
for developer convenience and increased productivity.

## Getting started

The build system uses CMake

### Dependencies

The following third party libraries are used internally:

 - CMake, for generating build system files (Unix Makefiles, Visual Studio files etc.)
 - libcurl, for HTTP communication with the Arrowhead service directory REST API
 - PugiXML, for XML parsing of results (optional)
 - Doxygen, for generating API documentation (optional)
 - log4cplus, for formatted debug output (optional)
 - C++ standard library
 - (bundled header-only, no install required) Catch C++ unit testing library

Planned future deps:

 - libcoap? for interfacing with CoAP services
 - ldns? for creating DNS-SD update requests
 - openssl? for signing DNS-SD updates and for encrypted communication
 - Some JSON lib? for speaking with JSON services

### Building the tests

basic Unix build:

    git clone https://github.com/gebart/arrowhead.git arrowhead
    cd arrowhead
    mkdir build
    cd build
    cmake ..
    make
    make test
    make doc

API documentation is generated using Doxygen.

## Documentation

Online API documentation can be found at http://gebart.github.io/arrowhead/api/index.html

Generating documentation locally is done with the `make doc` command.
