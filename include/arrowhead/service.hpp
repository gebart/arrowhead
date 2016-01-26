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
};

/** @} */

} /* namespace Arrowhead */
#endif /* ARROWHEAD_SERVICE_HPP_ */
