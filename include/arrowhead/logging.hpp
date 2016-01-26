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
 * @brief       Logging interface definitions
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_LOGGING_HPP_
#define ARROWHEAD_LOGGING_HPP_

#include <string>
#include <sstream>

#include "arrowhead/config.h"

#if ARROWHEAD_USE_LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/version.h>

#if LOG4CPLUS_VERSION >= LOG4CPLUS_MAKE_VERSION(2, 0, 0)
#include <log4cplus/initializer.h>
#endif

/**
 * @ingroup  logging
 *
 * @{
 */

/* macros mapping Arrowhead logging commands to log4cplus */

/**
 * @brief  Log message at log level "trace"
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_TRACE(logger, stream_expr) do { \
    LOG4CPLUS_TRACE((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Log message at log level "debug"
 *
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_DEBUG(logger, stream_expr) do { \
    LOG4CPLUS_DEBUG((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Log message at log level "info"
 *
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_INFO(logger, stream_expr) do { \
    LOG4CPLUS_INFO((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Log message at log level "warn"
 *
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_WARN(logger, stream_expr) do { \
    LOG4CPLUS_WARN((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Log message at log level "error"
 *
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_ERROR(logger, stream_expr) do { \
    LOG4CPLUS_ERROR((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Log message at log level "fatal"
 *
 * The stream expression will be piped to a std::stringstream using
 * `ss << stream_expr;`
 *
 * @param[in] logger       a Logger object
 * @param[in] stream_expr  a stream output expression
 */
#define ARROWHEAD_LIB_FATAL(logger, stream_expr) do { \
    LOG4CPLUS_FATAL((logger), static_cast<std::ostringstream&>(std::ostringstream() << stream_expr).str()); \
    } while(0)

/**
 * @brief  Get an instance to a logger
 *
 *
 * @note This macro is used to conditionally define logger variables depending
 * on whether log4cplus was enabled in the cmake configuration for the current build
 *
 * @param[in] logger_var    name of the logger object variable
 * @param[in] instance_name instance name as seen in the log
 */
#define ARROWHEAD_LIB_LOGGER(logger_var, instance_name) \
    auto logger_var = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(instance_name))

#else /* ARROWHEAD_USE_LOG4CPLUS */

/* stub macros */
#define ARROWHEAD_LIB_TRACE(logger, stream_expr)
#define ARROWHEAD_LIB_DEBUG(logger, stream_expr)
#define ARROWHEAD_LIB_INFO(logger, stream_expr)
#define ARROWHEAD_LIB_WARN(logger, stream_expr)
#define ARROWHEAD_LIB_ERROR(logger, stream_expr)
#define ARROWHEAD_LIB_FATAL(logger, stream_expr)
#define ARROWHEAD_LIB_LOGGER(logger_var, instance_name)

#endif /* ARROWHEAD_USE_LOG4CPLUS */

namespace Arrowhead {
namespace Logging {

/**
 * @brief  Configure the logging system
 *
 * If no file name is given (or an empty string), the logging system will be
 * configured using the default configuration in log4cplus::BasicConfigurator.
 * Otherwise, the given configuration file will be loaded using
 * log4cplus::PropertyConfigurator.
 *
 * @see [log4cplus::PropertyConfigurator::configure documentation](http://log4cplus.sourceforge.net/docs/html/classlog4cplus_1_1PropertyConfigurator.html)
 *
 * @param[in]  config_file  File name to a configuration file
 */
void configure(std::string config_file = std::string());

} // namespace Logging

/** @} */

} // namespace Arrowhead

#endif /* ARROWHEAD_LOGGING_HPP_ */
