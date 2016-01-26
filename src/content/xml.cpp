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
 * @brief       Arrowhead XML parsing helpers implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include <string>
#include <sstream>

#include "arrowhead/config.h"

#include <pugixml.hpp>

#include "arrowhead/exception.hpp"
#include "arrowhead/service.hpp"

namespace Arrowhead {

namespace XML {

namespace {

/**
 * @ingroup xml_detail
 * @{
 */

/**
 * @brief Generate a human readable error message from a PugiXML result object
 *
 * @param[in] result  result object from a previous PugiXML call
 *
 * @return    Human readable error message
 */
std::string xml_error_string(const pugi::xml_parse_result& result)
{
    std::stringstream ss;
    ss << "XMLError " << result.status <<
        ": " << result.description() <<
        " at offset " << result.offset;
    return ss.str();
}

/** @} */
} /* anonymous namespace */

void parse_buffer(pugi::xml_document& doc, const char *xmlbuf, size_t buflen)
{
    pugi::xml_parse_result result = doc.load_buffer(xmlbuf, buflen);

    if (result.status != pugi::status_ok) {
        std::string errmsg = xml_error_string(result);
        throw ContentError(errmsg);
    }
}

ServiceDescription to_service(const pugi::xml_node& srv)
{
    pugi::xpath_query inner_text(".");
    ServiceDescription sd;
    sd.name = inner_text.evaluate_string(srv.child("name"));
    sd.type = inner_text.evaluate_string(srv.child("type"));
    sd.domain = inner_text.evaluate_string(srv.child("domain"));
    sd.host = inner_text.evaluate_string(srv.child("host"));
    sd.port = inner_text.evaluate_number(srv.child("port"));
    for (auto child: srv.child("properties").children())
    {
        std::string name = inner_text.evaluate_string(child.child("name"));
        std::string value = inner_text.evaluate_string(child.child("value"));
        sd.properties[name] = value;
    }
    return sd;
}

} /* namespace XML */

} /* namespace Arrowhead */
