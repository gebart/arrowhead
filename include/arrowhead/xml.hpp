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
 * @brief       XML processing API declarations
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_XML_HPP_
#define ARROWHEAD_XML_HPP_

#include "arrowhead/config.h"

#include <cstddef> // for size_t

namespace Arrowhead {

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
template<class OutputIt, class StringType>
    ServiceDescription parse_service_xml(const StringType& xml_str);

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
template<class OutputIt>
    ServiceDescription parse_service_xml(const char *xmlbuf, size_t buflen);

/** @} */

} /* namespace Arrowhead */

/* Template definitions are found in detail/xml.hpp */
#include "arrowhead/detail/xml.hpp"

#endif /* ARROWHEAD_XML_HPP_ */

