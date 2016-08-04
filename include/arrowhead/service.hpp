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
 * @brief       Service representation definitions
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_SERVICE_HPP_
#define ARROWHEAD_SERVICE_HPP_

#include <string>
#include <map>

#include "arrowhead/config.h"

namespace Arrowhead {

/**
 * @ingroup  service
 *
 * @{
 */

/**
 * @brief Basic service information data structure
 */
struct ServiceDescription {
    /// Service name, e.g. `orchestration-mgmt._orch-m-ws-http._tcp.srv.arrowhead.ltu.se.`
    std::string name;
    /// Service type, e.g. `_orch-m-ws-http._tcp`
    std::string type;
    /// Domain, e.g. `arrowhead.ltu.se.`
    std::string domain;
    /// Host providing the service, e.g. `ns.arrowhead.ltu.se`
    std::string host;
    /// Port (TCP or UDP) where the service is available
    unsigned int port;
    /// Additional properties (key-value pairs)
    std::map<std::string, std::string> properties;

    /**
     * @ingroup  json
     * @{
     */

    /**
     * @brief Parse a JSON representation of a single service
     *
     * @param[in]    js_str  string containing a serialized JSON object
     *
     * @return ServiceDescription object with fields filled from the JSON content
     *
     * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
     *
     * @throws ContentError if there are any parsing errors
     */
    template<class StringType>
        static ServiceDescription from_json(const StringType& js_str);

    /**
     * @brief Parse a JSON representation of a single service
     *
     * @param[in]    jsbuf   C-string containing a serialized JSON object
     * @param[in]    buflen  length of @p jsbuf
     *
     * @return ServiceDescription object with fields filled from the JSON content
     *
     * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
     *
     * @throws ContentError if there are any parsing errors
     */
    static ServiceDescription from_json(const char *jsbuf, size_t buflen);

    /** @} */

    /**
     * @ingroup  xml
     * @{
     */

    /**
     * @brief Parse an XML representation of a single service
     *
     * @param[in]    xml_str XML document string containing a `<service>` tag
     *
     * @return ServiceDescription object with fields filled from the XML content
     *
     * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
     *
     * @throws ContentError if there are any XML parsing errors
     */
    template<class StringType>
        static ServiceDescription from_xml(const StringType& xml_str);

    /**
     * @brief Parse an XML representation of a single service
     *
     * @param[in]    xmlbuf  XML document C-string containing a `<service>` tag
     * @param[in]    buflen  length of xmlbuf
     *
     * @return ServiceDescription object with fields filled from the XML content
     *
     * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
     *
     * @throws ContentError if there are any XML parsing errors
     */
    static ServiceDescription from_xml(const char *xmlbuf, size_t buflen);

    /** @} */
};

/**
 * @ingroup  json
 * @{
 */

/**
 * @brief Parse a JSON representation of a service list and pass the parsed objects to @p oit
 *
 * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML

 * @param[in]    oit      Output iterator where the parsed objects will be placed
 * @param[in]    js_str   string containing a serialized JSON object
 *
 * @return Output iterator after outputting the objects
 *
 * @throws ContentError if there are any parsing errors
 */
template<class OutputIt, class StringType>
    OutputIt parse_servicelist_json(OutputIt oit, const StringType& js_str);

/**
 * @brief Parse a JSON representation of a service list and pass the parsed objects to @p oit
 *
 * @param[in]    oit     Output iterator where the parsed objects will be placed
 * @param[in]    jsbuf   C-string containing a serialized JSON object
 * @param[in]    buflen  length of @p jsbuf
 *
 * @return Output iterator after outputting the objects
 *
 * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
 *
 * @throws ContentError if there are any parsing errors
 */
template<class OutputIt>
    OutputIt parse_servicelist_json(OutputIt oit, const char *jsbuf, size_t buflen);

/** @} */

/**
 * @ingroup  xml
 * @{
 */

/**
 * @brief Parse an XML representation of a service list and pass the parsed objects to oit
 *
 * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML

 * @param[in]    oit     Output iterator where the parsed objects will be placed
 * @param[in]    xml_str XML document string containing a `<serviceList>` tag
 *
 * @return Output iterator after outputting the objects
 *
 * @throws ContentError if there are any XML parsing errors
 */
template<class OutputIt, class StringType>
    OutputIt parse_servicelist_xml(OutputIt oit, const StringType& xml_str);

/**
 * @brief Parse an XML representation of a service list and pass the parsed objects to oit
 *
 * @param[in]    oit     Output iterator where the parsed objects will be placed
 * @param[in]    xmlbuf  XML document C-string containing a `<serviceList>` tag
 * @param[in]    buflen  length of xmlbuf
 *
 * @return Output iterator after outputting the objects
 *
 * @see Arrowhead documentation ServiceDiscovery REST_HTTP_COAP-JSON-XML
 *
 * @throws ContentError if there are any XML parsing errors
 */
template<class OutputIt>
    OutputIt parse_servicelist_xml(OutputIt oit, const char *xmlbuf, size_t buflen);

/** @} */

/** @} */

} /* namespace Arrowhead */

/* Template definitions are found in detail/_service_*.hpp */
#include "arrowhead/detail/_service_json.hpp"
#include "arrowhead/detail/_service_xml.hpp"

#endif /* ARROWHEAD_SERVICE_HPP_ */
