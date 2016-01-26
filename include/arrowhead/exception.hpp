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
 * @brief       Exception classes
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_EXCEPTION_HPP_
#define ARROWHEAD_EXCEPTION_HPP_

#include <stdexcept>

#include "arrowhead/config.h"

namespace Arrowhead {

/**
 * @ingroup  exception
 * @brief    Top level exception class for all library-generated error exceptions
 */
class Error : public std::runtime_error {
    public:
        /**
         * @brief  Constructor
         *
         * @param[in]  msg Error message string
         */
        explicit Error(const std::string &msg):
            std::runtime_error(msg)
        {}
};

/**
 * @ingroup  exception
 * @brief    Transport error, something is wrong with the communication channel
 *
 * This includes all kinds of network errors and connection problems.
 */
class TransportError : public Error {
    public:
        /**
         * @brief  Constructor
         *
         * @param[in]  msg Error message string
         */
        explicit TransportError(const std::string &msg):
            Error(msg)
        {}
};

/**
 * @ingroup  exception
 * @brief    Content error, something is wrong with the content
 *
 * This includes content parsing errors, missing crucial information in a message etc.
 */
class ContentError : public Error {
    public:
        /**
         * @brief  Constructor
         *
         * @param[in]  msg Error message string
         */
        explicit ContentError(const std::string &msg):
            Error(msg)
        {}
};

} /* namespace Arrowhead */

#endif /* ARROWHEAD_EXCEPTION_HPP_ */
