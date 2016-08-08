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

#include <stdexcept> // for std::runtime_error
#include <new> // for std::bad_alloc
#include <cstring> // for strerror
#include <errno.h> // for errno

#if ARROWHEAD_USE_LIBCOAP
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

CoAPContext::CoAPContext(unsigned short port)
{
    coap_address_init(&listen_addr);
    listen_addr.addr.sin6.sin6_family = AF_INET6;
    listen_addr.addr.sin6.sin6_addr   = IN6ADDR_ANY_INIT;
    listen_addr.addr.sin6.sin6_port   = htons(port);
    ctx = coap_new_context(&listen_addr);
    if (ctx == 0) {
        // Failed to allocate a new context struct
        throw std::bad_alloc();
    }
}

CoAPContext::CoAPContext(const struct sockaddr_in6& sin6)
{
    coap_address_init(&listen_addr);
    listen_addr.addr.sin6 = sin6;
    ctx = coap_new_context(&listen_addr);
    if (ctx == 0) {
        // Failed to allocate a new context struct
        throw std::bad_alloc();
    }
}

CoAPContext::~CoAPContext()
{
    // Clean up context
    coap_free_context(ctx);
}

void CoAPContext::add_resource(coap_resource_t *res)
{
    coap_add_resource(ctx, res);
}

void CoAPContext::add_resource(CoAPResource& res)
{
    coap_add_resource(ctx, res.resource_ptr());
}

void CoAPContext::run_forever()
{
    using namespace std;
    fd_set readfds;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(ctx->sockfd, &readfds);
        int result = select(FD_SETSIZE, &readfds, 0, 0, 0);
        if (result < 0)
        {
            std::string err(strerror(errno));
            throw std::runtime_error("select: " + err);
        }
        else if (result > 0 && FD_ISSET(ctx->sockfd, &readfds))
        {
            coap_read(ctx);
        }
    }
}

void CoAPContext::perform_read()
{
    coap_read(ctx);
}

} /* namespace Arrowhead */
#endif
