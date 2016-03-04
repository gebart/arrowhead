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
 * @brief       Service Registry interface implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include "arrowhead/config.h"

#if ARROWHEAD_USE_LIBCURL

#include <string>
#include <iterator>
#include <sstream>
#include "arrowhead/core_services/serviceregistry.hpp"

namespace Arrowhead {

namespace {

/**
 * @brief  Perform a libcurl request and throw exception if an error occurs
 *
 * @param[in]  ctx  Context for the request
 *
 * @throw  TransportError if libcurl signals an error
 * @throw  TransportError if the HTTP response code is not 200
 */
void libcurl_perform_checked_throw(HTTP::CURLContext& ctx)
{
    /* Perform the request */
    CURLcode curl_code = curl_easy_perform(ctx.curl);

    /* Check for errors */
    if (curl_code != CURLE_OK) {
        std::ostringstream ss;
        ss << "CURLError " << curl_code <<
            ": " << curl_easy_strerror(curl_code) <<
            ", " << ctx.errbuf;
        throw TransportError(ss.str());
    }
    long http_code = 0;
    curl_easy_getinfo (ctx.curl, CURLINFO_RESPONSE_CODE, &http_code);
    if ((http_code < 200) || (http_code >= 299))
    {
        std::ostringstream ss;
        ss << "HTTPError " << http_code;
        /* Fail */
        throw TransportError(ss.str());
    }
}
}

std::string ServiceRegistryHTTP::list(std::string type)
{
    ARROWHEAD_LIB_LOGGER(logger, "ServiceRegistryHTTP::list");
    ARROWHEAD_LIB_TRACE(logger, "+ServiceRegistryHTTP::list");
    HTTP::CURLContext ctx;
    std::ostringstream buf;

    /* Set URL */
    std::string url = url_base;

    if (!type.empty()) {
        /* List only specific type of service */
        url += "/type/" + type;
    }
    else {
        /* List all services */
        url += "/service";
    }
    curl_easy_setopt(ctx.curl, CURLOPT_URL, url.c_str());

    /* Set Accept: header */
    ctx.add_header("Accept: application/xml");

    /* Set up callback */
    ctx.set_write_iterator(std::ostream_iterator<char>(buf));

    try {
        libcurl_perform_checked_throw(ctx);
    }
    catch (TransportError &e) {
        ARROWHEAD_LIB_ERROR(logger, e.what());
        ARROWHEAD_LIB_DEBUG(logger, std::string("Remote said: ") + buf.str());
        throw e;
    }

    /* Success, proceed with parsing XML */
    return buf.str();
}

std::string ServiceRegistryHTTP::publish(ServiceDescription service)
{
    ARROWHEAD_LIB_LOGGER(logger, "ServiceRegistryHTTP::publish");
    ARROWHEAD_LIB_TRACE(logger, "+ServiceRegistryHTTP::publish");
    HTTP::CURLContext ctx;
    std::ostringstream buf;
    std::ostringstream postdata;

    /* Create request data */
    postdata << "<service>";
    postdata
        << "<domain>" << service.domain << "</domain>"
        << "<host>" << service.host << "</host>"
        << "<name>" << service.name << "</name>"
        << "<port>" << service.port << "</port>"
        << "<type>" << service.type << "</type>";

    postdata << "<properties>";
    for (auto& kv: service.properties) {
        postdata << "<property>"
            << "<name>" << kv.first << "</name>"
            << "<value>" << kv.second << "</value>"
            << "</property>";
    }
    postdata << "</properties>";

    postdata << "</service>";

    std::string poststr = postdata.str();

    ARROWHEAD_LIB_DEBUG(logger, "POST: " << poststr);

    /* Set POST data */
    curl_easy_setopt(ctx.curl, CURLOPT_POSTFIELDS, poststr.c_str());
    /* if we don't provide POSTFIELDSIZE, libcurl will call strlen() by itself */
    curl_easy_setopt(ctx.curl, CURLOPT_POSTFIELDSIZE, poststr.size());

    /* Set URL */
    std::string url = url_base + "/publish";
    curl_easy_setopt(ctx.curl, CURLOPT_URL, url.c_str());

    /* Set Accept: header */
    ctx.add_header("Accept: application/xml");

    /* Set Content-Type: header */
    ctx.add_header("Content-Type: application/xml");

    /* Set up callback */
    ctx.set_write_iterator(std::ostream_iterator<char>(buf));

    try {
        libcurl_perform_checked_throw(ctx);
    }
    catch (TransportError &e) {
        ARROWHEAD_LIB_ERROR(logger, e.what());
        ARROWHEAD_LIB_DEBUG(logger, std::string("Remote said: ") + buf.str());
        throw e;
    }

    /* Success, proceed with parsing XML */
    ARROWHEAD_LIB_TRACE(logger, "-ServiceRegistryHTTP::publish");
    return buf.str();
}

std::string ServiceRegistryHTTP::unpublish(std::string name)
{
    ARROWHEAD_LIB_LOGGER(logger, "ServiceRegistryHTTP::unpublish");
    ARROWHEAD_LIB_TRACE(logger, "+ServiceRegistryHTTP::unpublish");
    HTTP::CURLContext ctx;
    std::ostringstream buf;
    std::ostringstream postdata;

    /* Create request data, only the name is needed to unpublish something */
    postdata << "<service>"
        << "<name>" << name << "</name>"
        << "</service>";

    std::string poststr = postdata.str();

    ARROWHEAD_LIB_DEBUG(logger, "POST: " << poststr);

    /* Set POST data */
    curl_easy_setopt(ctx.curl, CURLOPT_POSTFIELDS, poststr.c_str());
    /* if we don't provide POSTFIELDSIZE, libcurl will call strlen() by itself */
    curl_easy_setopt(ctx.curl, CURLOPT_POSTFIELDSIZE, poststr.size());

    /* Set URL */
    std::string url = url_base + "/unpublish";
    curl_easy_setopt(ctx.curl, CURLOPT_URL, url.c_str());

    /* Set Accept: header */
    ctx.add_header("Accept: application/xml");

    /* Set Content-Type: header */
    ctx.add_header("Content-Type: application/xml");

    /* Set up callback */
    ctx.set_write_iterator(std::ostream_iterator<char>(buf));

    try {
        libcurl_perform_checked_throw(ctx);
    }
    catch (TransportError &e) {
        ARROWHEAD_LIB_ERROR(logger, e.what());
        ARROWHEAD_LIB_DEBUG(logger, std::string("Remote said: ") + buf.str());
        throw e;
    }

    /* Success, proceed with parsing XML */
    ARROWHEAD_LIB_TRACE(logger, "-ServiceRegistryHTTP::unpublish");
    return buf.str();
}


} /* namespace Arrowhead */

#endif /* ARROWHEAD_USE_LIBCURL */
