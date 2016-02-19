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
 * @brief       HTTP communication implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include "arrowhead/config.h"

#if ARROWHEAD_USE_LIBCURL

#include <stdexcept>
#include <curl/curl.h>
#include "arrowhead/http.hpp"

/**
 * @ingroup  http
 *
 * @brief    HTTP user agent string passed to the server
 */
#define HTTP_USERAGENT_STRING "libarrowhead/0.1"

namespace Arrowhead {

namespace HTTP {

namespace {

/**
 * @ingroup http_detail
 * @{
 */

/**
 * @brief  C wrapper for the C++ ACURLCallback interface class
 *
 * This wrapping is necessary in order to call C++ object methods from C
 *
 * @param[in]  ptr      Pointer to the received data
 * @param[in]  size     Bytes per block
 * @param[in]  nmemb    Number of blocks
 * @param[in]  userdata User data pointer, used for a pointer to the ACURLCallback object
 *
 * @return  obj->callback(ptr, size, nmemb)
 */
extern "C" size_t curl_callback_wrapper(char *ptr, size_t size, size_t nmemb, void *userdata) {
    ACURLCallback* obj = reinterpret_cast<ACURLCallback*>(userdata);
    return obj->callback(ptr, size, nmemb);
}

/** @} */
} // anonymous namespace

CURLContext::CURLContext() : curl(curl_easy_init()), headers(NULL), write_cb(NULL)
{
    /* Verify initialization went OK */
    if (curl == NULL) {
        throw TransportError("curl_easy_init() failed!");
    }
    /* provide a buffer to store errors in */
    errbuf[0] = '\0';
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

    /* Set some default settings */
    /* follow HTTP Location: headers */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    if (curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_USERAGENT_STRING) != CURLE_OK) {
        throw std::bad_alloc();
    }

    /* Set up write callback */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback_wrapper);
}

CURLContext::~CURLContext()
{
    delete write_cb;
    write_cb = NULL;
    /* always cleanup */
    curl_slist_free_all(headers);
    headers = NULL;
    curl_easy_cleanup(curl);
    curl = NULL;
}

void CURLContext::add_header(const char *str)
{
    headers = curl_slist_append(headers, str);
    if (headers == NULL) {
        throw std::bad_alloc();
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

} // namespace HTTP
} // namespace Arrowhead

#endif /* ARROWHEAD_USE_LIBCURL */
