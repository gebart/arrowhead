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
 * @brief       Logging implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include "arrowhead/logging.hpp"

#if ARROWHEAD_USE_LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/version.h>
#endif /* ARROWHEAD_USE_LOG4CPLUS */

namespace Arrowhead {
namespace Logging {

void configure(std::string config_file)
{
#if ARROWHEAD_USE_LOG4CPLUS
    if (config_file.empty()) {
        log4cplus::BasicConfigurator::doConfigure();
    }
    else {
        try {
            // Try to read logging configuration file
            log4cplus::PropertyConfigurator::doConfigure(config_file);
        }
        catch(std::exception& e) {
            auto root = log4cplus::Logger::getRoot();
            ARROWHEAD_LIB_ERROR(root, "Exception occurred while loading '" << config_file << "': " << e.what());
        }
    }
#else /* ARROWHEAD_USE_LOG4CPLUS */
    // stub!
    (void) config_file;
#endif /* ARROWHEAD_USE_LOG4CPLUS */
}

} // namespace Logging
} // namespace Arrowhead
