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
 * @brief       HTTP CURL wrapper template definitions
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_DETAIL_HTTP_HPP_
#define ARROWHEAD_DETAIL_HTTP_HPP_

#include <cstddef>
#include <algorithm>
#include <curl/curl.h>
#include "arrowhead/exception.hpp"
#include "arrowhead/logging.hpp"

namespace Arrowhead {

/**
 * @ingroup  http_detail
 *
 * @{
 */

namespace HTTP {

/**
 * @ingroup http_detail
 * @brief   CURL callback class template
 */
template<class OutputIterator> class CURLCallback : public ACURLCallback {
    private:
        OutputIterator oit;
    public:
        /**
         * @brief  Constructor
         *
         * @param[in]  oit Output iterator to write the callback data to
         */
        CURLCallback(OutputIterator oit) : oit(oit) {}

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
        virtual size_t callback(char *ptr, size_t size, size_t nmemb) {
            try {
                ARROWHEAD_LIB_LOGGER(logger, "curl_write_callback");

                size_t nbytes = size * nmemb;
                std::copy_n(ptr, nbytes, oit);
                ARROWHEAD_LIB_DEBUG(logger, "Received " << nbytes << " bytes...");
                return nbytes;
            }
            catch (...) {
                // It's not safe to throw exceptions across C functions (libcurl)
                // because C is not aware of the exceptions and may not perform
                // the necessary cleanup.
                return 0;
            }
        }
};

template<class StringType> void CURLContext::add_header(const StringType& str)
{
    add_header(str.c_str());
}

template<class OutputIterator> void CURLContext::set_write_iterator(OutputIterator oit)
{
    delete write_cb;
    write_cb = NULL;
    write_cb = new CURLCallback<OutputIterator>(oit);

    /* Set up callback */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(write_cb));
}

} /* namespace HTTP */

/** @} */

} /* namespace Arrowhead */
#endif /* ARROWHEAD_DETAIL_HTTP_HPP_ */
