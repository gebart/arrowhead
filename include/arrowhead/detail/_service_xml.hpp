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
 * @brief       XML processing template definitions
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_DETAIL_SERVICE_XML_HPP_
#define ARROWHEAD_DETAIL_SERVICE_XML_HPP_

#include <cstddef> // for size_t
#include <sstream>

#include "arrowhead/config.h"

#if ARROWHEAD_USE_PUGIXML
#include <pugixml.hpp>
#endif

#include "arrowhead/exception.hpp"
#include "arrowhead/service.hpp"

namespace Arrowhead {

namespace XML {

/**
 * @ingroup xml_detail
 * @{
 */

#if ARROWHEAD_USE_PUGIXML

/**
 * @internal
 * @brief Parse document and throw exception if any errors occur.
 *
 * @param[out]  doc     XML document object for containing the parsed tree
 * @param[in]   xmlbuf  buffer containing XML data
 * @param[in]   buflen  length of buffer, in bytes
 *
 * @throws Arrowhead::ContentError If the buffer could not be parsed
 */
void parse_buffer(pugi::xml_document& doc, const char *xmlbuf, size_t buflen);

#endif /* ARROWHEAD_USE_PUGIXML */

#if ARROWHEAD_USE_PUGIXML

/**
 * @internal
 * @brief Translate a single XML `<service>` node into a ServiceDescription
 *
 * @param[in] srv  A `<service>` XML node object
 *
 * @return ServiceDescription object with fields filled from the XML content
 */
ServiceDescription service_from_node(const pugi::xml_node& srv);

#endif /* ARROWHEAD_USE_PUGIXML */

/** @} */

} /* namespace XML */

/* Implementations of templates declared in include/arrowhead/service.hpp */

template<class StringType>
    ServiceDescription ServiceDescription::from_xml(const StringType& xml_str)
{
    return ServiceDescription::from_xml(xml_str.c_str(), xml_str.size());
}

template<class OutputIt, class StringType>
    OutputIt parse_servicelist_xml(OutputIt oit, const StringType& xml_str)
{
    return parse_servicelist_xml(oit, xml_str.c_str(), xml_str.size());
}

#if ARROWHEAD_USE_PUGIXML

template<class OutputIt>
    OutputIt parse_servicelist_xml(OutputIt oit,
        const char *xmlbuf, size_t buflen)
{
    pugi::xml_document doc;
    XML::parse_buffer(doc, xmlbuf, buflen);

    auto listnode = doc.child("serviceList");
    if (listnode) {
        for (auto srv: listnode.children("service")) {
            ServiceDescription sd = XML::service_from_node(srv);
            *oit++ = sd;
        }
    }

    return oit;
}
#endif /* ARROWHEAD_USE_PUGIXML */

} /* namespace Arrowhead */
#endif /* ARROWHEAD_DETAIL_SERVICE_XML_HPP_ */

