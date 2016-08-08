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
 * @brief       CoAP interface
 *
 * @author      Joakim Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_COAP_HPP_
#define ARROWHEAD_COAP_HPP_

#include "arrowhead/config.h"

#include <string> // for std::string
#include <netinet/in.h> // For sockaddr_in6 et. al

#if ARROWHEAD_USE_LIBCOAP
#include "coap/coap.h"
#endif /* ARROWHEAD_USE_LIBCOAP */

namespace Arrowhead {

/**
 * @ingroup  coap
 *
 * @{
 */

/**
 * @brief represents a single resource in a CoAP server
 */
class CoAPResource {
    public:
        /**
         * @brief Constructor
         *
         * @param[in]  uri  URI path to the resource on the server
         *
         * @throws std::bad_alloc if memory allocation fails
         */
        template<class StringType>
            CoAPResource(const StringType& uri) : uri(uri)
        {
            initialize();
        }

        // Disable copying for now
        CoAPResource(CoAPResource const&) = delete;
        CoAPResource& operator=(CoAPResource const&) = delete;

#if ARROWHEAD_USE_LIBCOAP
        void register_handler(unsigned char method, coap_method_handler_t handler);
#endif /* ARROWHEAD_USE_LIBCOAP */

#if ARROWHEAD_USE_LIBCOAP
        coap_resource_t *resource_ptr()
        {
            return res;
        }
#endif /* ARROWHEAD_USE_LIBCOAP */

    private:
        /**
         * @internal
         * @brief Create a new libcoap context from the existing URI variable.
         *
         * @throws std::bad_alloc if memory allocation fails
         */
        void initialize();

#if ARROWHEAD_USE_LIBCOAP
        /**
         * @internal
         * @brief Pointer to libcoap resource
         */
        coap_resource_t *res;
#endif /* ARROWHEAD_USE_LIBCOAP */

        /**
         * @internal
         * @brief String representation of URI-Path
         *
         * @note This must not be modified after coap_resource_init has been
         *       called, else undefined behaviour. Hence @b const
         */
        const std::string uri;
};

class CoAPContext {
    public:
        CoAPContext(unsigned short port = 0);

        CoAPContext(const struct sockaddr_in6& sin6);

        ~CoAPContext();

        // Disable copying for now
        CoAPContext(CoAPContext const&) = delete;
        CoAPContext& operator=(CoAPContext const&) = delete;

#if ARROWHEAD_USE_LIBCOAP
        void add_resource(coap_resource_t *res);
#endif /* ARROWHEAD_USE_LIBCOAP */
        void add_resource(CoAPResource& res);


        void run_forever();

        void run_once();

    private:
#if ARROWHEAD_USE_LIBCOAP
        coap_context_t *ctx;
        coap_address_t  listen_addr;
#endif /* ARROWHEAD_USE_LIBCOAP */
};

/** @} */

} /* namespace Arrowhead */

#endif /* ARROWHEAD_COAP_HPP_ */
