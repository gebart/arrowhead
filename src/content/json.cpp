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
 * @brief       Arrowhead JSON parsing helpers implementation
 *
 * @author      Joakim Nohlgård <joakim@nohlgard.se>
 */

#include <string>
#include "arrowhead/config.h"

#if ARROWHEAD_USE_JSON

#include "arrowhead/service.hpp"

#include "nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;

namespace Arrowhead {

namespace JSON {

ServiceDescription service_from_obj(const json& srv)
{
    ServiceDescription sd;
    sd.name = srv["name"];
    sd.type = srv["type"];
    sd.domain = srv["domain"];
    sd.host = srv["host"];
    sd.port = srv["port"];
    json props = srv["properties"]["property"];
    for (auto it = props.begin(); it != props.end(); ++it)
    {
        std::string name = (*it)["name"];
        std::string value = (*it)["value"];
        sd.properties[name] = value;
    }
    return sd;
}

} /* namespace JSON */

ServiceDescription ServiceDescription::from_json(const char *jsbuf, size_t buflen)
{
    const std::string js_str(jsbuf, buflen);
    return ServiceDescription::from_json(js_str);
}

} /* namespace Arrowhead */

#endif /* ARROWHEAD_USE_JSON */
