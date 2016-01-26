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
 * @brief       Service registry list query tests implementation
 *
 * @todo        Add robust testing against a real server
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include "catch.hpp"
#include "arrowhead/core_services/serviceregistry.hpp"
#include <vector>
#include <iterator>

SCENARIO( "Service registry REST API", "[serviceregistry]" ) {
    GIVEN("a ServiceRegistryHTTP instance with a nonexistent URL") {
        Arrowhead::ServiceRegistryHTTP reg("http://non-existent-domain.broken/services");

        WHEN("a service list is requested") {
            THEN("the object throws TransportError") {
                REQUIRE_THROWS_AS(reg.list(), Arrowhead::TransportError);
            }
        }
    }
    GIVEN("a ServiceRegistryHTTP instance with a proper URL") {
        Arrowhead::ServiceRegistryHTTP reg("http://ns.arrowhead.ltu.se:8045/servicediscovery");

        WHEN("a service list is requested") {
            THEN("a non-empty string is returned") {
                std::string str = reg.list();
                REQUIRE(!str.empty());
            }
        }
    }
}
