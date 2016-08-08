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

#include <netinet/in.h> // For sockaddr_in6

#include <string>
#include <boost/asio.hpp>

#if !ARROWHEAD_USE_LIBCOAP
#error coap.hpp requires libarrowhead built with libcoap support!
#endif /* !ARROWHEAD_USE_LIBCOAP */

#include "coap/coap.h"

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

        /**
         * @brief Register a handler function for the given method
         */
        void register_handler(unsigned char method, coap_method_handler_t handler);

        /**
         * @brief Get a pointer to the underlying libcoap resource
         */
        coap_resource_t *resource_ptr()
        {
            return res;
        }

    private:
        /**
         * @internal
         * @brief Create a new libcoap context from the existing URI variable.
         *
         * @throws std::bad_alloc if memory allocation fails
         */
        void initialize();

        /**
         * @internal
         * @brief Pointer to libcoap resource
         */
        coap_resource_t *res;

        /**
         * @internal
         * @brief String representation of URI-Path
         *
         * @note This must not be modified after coap_resource_init has been
         *       called, else undefined behaviour. Hence @b const
         */
        const std::string uri;
};

/**
 * @brief CoAP context
 *
 * libcoap requires a context for most operations. A context contains
 * information about the CoAP environment, such as network socket, provided
 * resources etc.
 */
class CoAPContext {
    public:
        /**
         * @brief Construct a libcoap context using the specified UDP port
         *
         * The socket will be listening on all IPv4+IPv6 interfaces
         *
         * @param[in]  io_service  io_service object to manage the socket
         * @param[in]  port        UDP port number
         */
        CoAPContext(boost::asio::io_service& io_service, unsigned short port = 0);

        /**
         * @brief Construct a libcoap context using the given socket parameters
         *
         * @param[in]  io_service  io_service object to manage the socket
         * @param[in]  sin6        IPv6 socket address parameters to bind to
         */
        CoAPContext(boost::asio::io_service& io_service, const struct sockaddr_in6& sin6);

        /**
         * @brief Clean up and free libcoap allocated memory
         */
        ~CoAPContext();

        // Disable copying for now
        CoAPContext(CoAPContext const&) = delete;
        CoAPContext& operator=(CoAPContext const&) = delete;

        /**
         * @brief Add a resource to the context
         *
         * wrapper for coap_add_resource
         *
         * @param[in]  res  resource to add
         */
        void add_resource(coap_resource_t *res);

        /**
         * @copydoc CoAPContext::add_resource(coap_resource_t*)
         */
        void add_resource(CoAPResource& res);

        /**
         * @brief Read packets from the bound socket and handle them
         *
         * wrapper for coap_read
         */
        void perform_read();

    private:
        /**
         * @internal
         * @brief Set up internal state
         *
         * Called from the constructors to reduce the amount of copy+paste code
         */
        void initialize();

        /**
         * @internal
         * @brief Handle all current libcoap events
         */
        void perform_operations();

        /**
         * @internal
         * @copydoc CoAPContext::perform_read()
         *
         * @param[in]  ec  Error code from Boost::Asio
         */
        void handle_read(const boost::system::error_code& ec);

        coap_context_t *ctx;
        coap_address_t  listen_addr;
        boost::asio::ip::udp::socket socket;
        bool read_queued;
};

/** @} */

} /* namespace Arrowhead */

#endif /* ARROWHEAD_COAP_HPP_ */
