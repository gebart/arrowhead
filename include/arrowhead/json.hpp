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
 * @brief       JSON processing API declarations
 *
 * @author      Joakim Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_JSON_HPP_
#define ARROWHEAD_JSON_HPP_

#include "arrowhead/config.h"

#include <cstddef> // for size_t

namespace Arrowhead {

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
    ServiceDescription parse_service_json(const StringType& js_str);

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
ServiceDescription parse_service_json(const char *jsbuf, size_t buflen);

/** @} */

} /* namespace Arrowhead */

/* Template definitions are found in detail/json.hpp */
#include "arrowhead/detail/json.hpp"

#endif /* ARROWHEAD_JSON_HPP_ */

