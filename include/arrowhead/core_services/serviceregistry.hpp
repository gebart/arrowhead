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
 * @brief       Arrowhead Service Registry interface classes
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#ifndef ARROWHEAD_CORE_SERVICES_SERVICEREGISTRY_HPP_
#define ARROWHEAD_CORE_SERVICES_SERVICEREGISTRY_HPP_

#include <string>

#include "arrowhead/config.h"

#include "arrowhead/service.hpp"
#include "arrowhead/http.hpp"
#include "arrowhead/logging.hpp"

namespace Arrowhead {

/**
 * @ingroup core_services
 * @{
 */

/**
 * @brief Service Registry HTTP REST API interface
 */
class ServiceRegistryHTTP {
    private:

    private:
        std::string url_base;

    public:
        /**
         * @brief Constructor
         *
         * @param[in] url_base Base URL for the service registry REST API
         */
        ServiceRegistryHTTP(std::string url_base)
            : url_base(url_base)
        {}

        /**
         * @brief List all services of the given type, or all services if type is empty
         *
         * @param[in] type   Service type, use "" to list all services of any type
         *
         * @return HTTP response content (XML string)
         */
        std::string list(std::string type = std::string());

        /**
         * @brief Publish the given service in the service registry
         *
         * @param[in] service   Service description to publish
         *
         * @return HTTP response content
         */
        std::string publish(ServiceDescription service);

        /**
         * @brief Publish the given service in the service registry
         *
         * @param[in] name   Name of the service to unpublish, as seen in list()
         *
         * @return HTTP response content
         */
        std::string unpublish(std::string name);
};

} /* namespace Arrowhead */
#endif /* ARROWHEAD_CORE_SERVICES_SERVICEREGISTRY_HPP_ */
