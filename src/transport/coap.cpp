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
 * @brief       Arrowhead CoAP implementation
 *
 * @todo split coap.cpp into file per class
 *
 * @author      Joakim Nohlgård <joakim@nohlgard.se>
 */

#include "arrowhead/config.h"

#if ARROWHEAD_USE_LIBCOAP

#include <new>              // for std::bad_alloc
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "arrowhead/coap.hpp"

namespace Arrowhead {

void CoAPResource::register_handler(unsigned char method, coap_method_handler_t handler)
{
    coap_register_handler(res, method, handler);
}

void CoAPResource::initialize()
{
    const unsigned char *urip = reinterpret_cast<const unsigned char *>(uri.c_str());
    int flags = 0;
    res = coap_resource_init(urip, uri.length(), flags);
    if (res == 0) {
        // Failed to allocate memory for resource descriptor
        throw std::bad_alloc();
    }
}

/* CoAPContext ********************** */

CoAPContext::CoAPContext(boost::asio::io_service& io_service, unsigned short port) :
    socket(io_service), read_queued(false)
{
    coap_address_t listen_addr;
    coap_address_init(&listen_addr);
    listen_addr.addr.sin6.sin6_family = AF_INET6;
    listen_addr.addr.sin6.sin6_addr   = IN6ADDR_ANY_INIT;
    listen_addr.addr.sin6.sin6_port   = htons(port);
    initialize(listen_addr);
}

CoAPContext::CoAPContext(boost::asio::io_service& io_service, const struct sockaddr_in6& sin6) :
    socket(io_service), read_queued(false)
{
    coap_address_t listen_addr;
    coap_address_init(&listen_addr);
    listen_addr.addr.sin6 = sin6;
    initialize(listen_addr);
}

CoAPContext::~CoAPContext()
{
    // Clean up context
    coap_free_context(ctx);
}

void CoAPContext::initialize(const coap_address_t& listen_addr)
{
    ctx = coap_new_context(&listen_addr);
    if (ctx == 0) {
        // Failed to allocate a new context struct
        throw std::bad_alloc();
    }
    // Set up Boost socket object
    socket.assign(boost::asio::ip::udp::v6(), ctx->sockfd);
    // Put the socket into non-blocking mode.
    socket.non_blocking(true);
    // Enqueue an asynchronous socket read
    perform_operations();
}

void CoAPContext::perform_operations()
{
    if (!read_queued) {
        read_queued = true;
        socket.async_receive(
            boost::asio::null_buffers(),
            boost::bind(&CoAPContext::handle_read, this,
                boost::asio::placeholders::error));

    }
}

void CoAPContext::add_resource(coap_resource_t *res)
{
    coap_add_resource(ctx, res);
}

void CoAPContext::add_resource(CoAPResource& res)
{
    coap_add_resource(ctx, res.resource_ptr());
}

void CoAPContext::perform_read()
{
    coap_read(ctx);
}

void CoAPContext::handle_read(const boost::system::error_code& ec)
{
    read_queued = false;
    if (!ec) {
        // call libcoap to let it know that a read is possible
        perform_read();
    }
    if (!ec || ec == boost::asio::error::would_block) {
        // Start over with a new async read
        perform_operations();
    }
    else {
        // Socket error, or something
        throw boost::system::system_error(ec);
    }
}

} /* namespace Arrowhead */
#endif /* ARROWHEAD_USE_LIBCOAP */
