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
 * @brief       HTTP CURL interface
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_HTTP_HPP_
#define ARROWHEAD_HTTP_HPP_

#include <cstddef>

#include "arrowhead/config.h"

#include <curl/curl.h>

namespace Arrowhead {

/**
 * @ingroup  http
 *
 * @{
 */

namespace HTTP {

/**
 * @brief  Abstract CURL callback interface
 */
class ACURLCallback {
    public:

        /**
         * @brief Callback function that can be called from CURL during transfers
         *
         * This method will be called from libcurl (via curl_callback_wrapper())
         * when receiving data from a server
         *
         * @param[in]  ptr      Pointer to the received data
         * @param[in]  size     Bytes per block
         * @param[in]  nmemb    Number of blocks
         *
         * @return     Number of bytes written
         */
        virtual size_t callback(char *ptr, size_t size, size_t nmemb) = 0;

        /**
         * @brief  Virtual destructor
         */
        virtual ~ACURLCallback() {};
};


/**
 * @brief CURL context wrapper class
 */
class CURLContext {
    public:
        /// CURL instance
        CURL *curl;
        /// Buffer for CURL error messages
        char errbuf[CURL_ERROR_SIZE];
        /// HTTP headers
        struct curl_slist *headers;
        /// Write callback
        ACURLCallback* write_cb;

        CURLContext();
        ~CURLContext();

        /**
         * @brief Add a header to the outgoing request (C string variant)
         *
         * This is a wrapper around curl_slist_append and curl_easy_setopt(CURLOPT_HTTPHEADER)
         *
         * @param[in]  str Header string to add
         */
        void add_header(const char *str);
        /**
         * @brief Overloaded add_header to support C++ strings and similar classes
         *
         * @param[in]  str A std::string-like string containing the header to add
         */
        template<class StringType> void add_header(const StringType& str);

        /**
         * @brief Set the given iterator to receive the remote response
         *
         * The remote response content will be written to oit, possibly in pieces
         *
         * @param[in]  oit  Output iterator where the received data will be written
         */
        template<class OutputIterator> void set_write_iterator(OutputIterator oit);
};

} /* namespace HTTP */

/** @} */

} /* namespace Arrowhead */

#include "arrowhead/detail/http.hpp"

#endif /* ARROWHEAD_HTTP_HPP_ */
