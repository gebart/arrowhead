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
 * @brief       JSON processing template definitions
 *
 * @author      Joakim Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_DETAIL_JSON_HPP_
#define ARROWHEAD_DETAIL_JSON_HPP_

#include <cstddef> // for size_t
#include <sstream>

#include "arrowhead/config.h"

#if ARROWHEAD_USE_JSON
#include "nlohmann/json.hpp"
#endif

#include "arrowhead/exception.hpp"
#include "arrowhead/logging.hpp"
#include "arrowhead/service.hpp"

namespace Arrowhead {

namespace JSON {

/**
 * @ingroup xml_detail
 * @{
 */

#if ARROWHEAD_USE_JSON
/**
 * @brief Translate a single JSON service object into a ServiceDescription
 *
 * @param[in] srv  A JSON object
 *
 * @return ServiceDescription object with fields filled from the JSON content
 */
ServiceDescription to_service(const nlohmann::json& srv);

#endif /* ARROWHEAD_USE_JSON */

/** @} */

} /* namespace JSON */

/* Implementations of templates declared in include/arrowhead/json.hpp */
#if ARROWHEAD_USE_JSON
template<class OutputIt, class StringType>
    OutputIt parse_servicelist_json(OutputIt oit, const StringType& js_str)
{
    nlohmann::json js = nlohmann::json::parse(js_str);

    nlohmann::json srvl = js["service"];

    for (auto it = srvl.begin(); it != srvl.end(); ++it)
    {
        ServiceDescription srv = JSON::to_service(*it);
        *oit++ = srv;
    }

    return oit;
}
#endif /* ARROWHEAD_USE_JSON */

#if ARROWHEAD_USE_JSON
template<class StringType>
    ServiceDescription parse_service_json(const StringType& js_str)
{
    nlohmann::json js = nlohmann::json::parse(js_str);
    return JSON::to_service(js);
}
#endif /* ARROWHEAD_USE_JSON */

template<class OutputIt>
    OutputIt parse_servicelist_json(OutputIt oit,
        const char *jsbuf, size_t buflen)
{
    const std::string js_str(jsbuf, buflen);
    return parse_servicelist_json(oit, js_str);
}

} /* namespace Arrowhead */
#endif /* ARROWHEAD_DETAIL_JSON_HPP_ */

