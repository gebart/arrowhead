/*
 * Copyright (c) 2015-2016 Fotonic
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Apache License v2.0 which accompanies this distribution.
 *
 *     The Eclipse Public License is available at
 *       http://www.eclipse.org/legal/epl-v10.html
 *
 *     The Apache License v2.0 is available at
 *       http://www.opensource.org/licenses/apache2.0.php
 *
 * You can redistribute this code under either of these licenses.
 * For more information; see http://www.arrowhead.eu/licensing
 */

/**
 * @file
 * @brief       Test applications main program entry point
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "arrowhead/config.h"

#if ARROWHEAD_USE_LIBCURL
#include <curl/curl.h>
#endif

#include "arrowhead/logging.hpp"

/**
 * @brief  Unit tests common entry point
 *
 * @param[in]  argc Command line argument count
 * @param[in]  argv Command line arguments
 *
 * @return 0 on success
 * @return non-zero if something went wrong
 * @return non-zero if a test failed
 */
int main(int argc, char* const argv[])
{
    Catch::Session session; // There must be exactly once instance

#if ARROWHEAD_USE_LIBCURL
    // Initialize libcurl globals first of all, this function must only be
    // called once and is not thread safe and may not be called when any other
    // threads are running
    curl_global_init(CURL_GLOBAL_ALL);
#endif

    // Initialize default logging, this prevents error messages about
    // uninitialized logging when running the tests
    Arrowhead::Logging::configure();

    // writing to session.configData() here sets defaults
    // this is the preferred way to set them

    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 ) {
        // Indicates a command line error
        return returnCode;
    }

    // writing to session.configData() or session.Config() here
    // overrides command line args
    // only do this if you know you need to

    return session.run();
}
