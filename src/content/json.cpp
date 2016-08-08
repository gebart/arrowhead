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

namespace Arrowhead {

namespace JSON {

ServiceDescription service_from_obj(const nlohmann::json& srv)
{
    ServiceDescription sd;
    sd.name = srv["name"];
    sd.type = srv["type"];
    sd.domain = srv["domain"];
    sd.host = srv["host"];
    sd.port = srv["port"];
    nlohmann::json props = srv["properties"]["property"];
    for (auto it = props.begin(); it != props.end(); ++it)
    {
        std::string name = (*it)["name"];
        std::string value = (*it)["value"];
        sd.properties[name] = value;
    }
    return sd;
}

nlohmann::json obj_from_service(const ServiceDescription& sd)
{
    using nlohmann::json; // Convenience
    json js;
    js["name"]   = sd.name;
    js["type"]   = sd.type;
    js["host"]   = sd.host;
    js["domain"] = sd.domain;
    js["port"]   = sd.port;
    // Silly format, change the below monstrosity to simply
    // js["properties"] = sd.properties;
    // when/if the format specification is updated
    js["properties"] = json::object();
    std::vector<std::map<std::string, std::string> > props;
    for (auto it = sd.properties.begin(); it != sd.properties.end(); ++it)
    {
        std::map<std::string, std::string> prop;
        prop["name"] = it->first;
        prop["value"] = it->second;
        props.push_back(prop);
    }
    js["properties"]["property"] = props;
    return js;
}

} /* namespace JSON */

ServiceDescription ServiceDescription::from_json(const char *jsbuf, size_t buflen)
{
    const std::string js_str(jsbuf, buflen);
    return ServiceDescription::from_json(js_str);
}

} /* namespace Arrowhead */

#endif /* ARROWHEAD_USE_JSON */
