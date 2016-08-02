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
 * @brief       Service registry XML parsing tests implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 */

#include "catch.hpp"
#include "arrowhead/service.hpp"
#include "arrowhead/json.hpp"
#include <vector>
#include <iterator>

#define TEST_JSON_LIST_2_SERVICES_TEXT \
    "{\n" \
    "    \"service\": [\n" \
    "        {\n" \
    "            \"name\": \"orchestration-store._orch-s-ws-https._tcp.srv.arces.unibo.it.\",\n" \
    "            \"type\": \"_orch-s-ws-https._tcp\",\n" \
    "            \"domain\": \"arces.unibo.it.\",\n" \
    "            \"host\": \"bedework.arces.unibo.it.\",\n" \
    "            \"port\": 8181,\n" \
    "            \"properties\": {\n" \
    "                \"property\": [\n" \
    "                    {\n" \
    "                        \"name\": \"version\",\n" \
    "                        \"value\": \"1.1\"\n" \
    "                    },\n" \
    "                    {\n" \
    "                        \"name\": \"path\",\n" \
    "                        \"value\": \"/orchestration/store/\"\n" \
    "                    }\n" \
    "                ]\n" \
    "            }\n" \
    "        },\n" \
    "        {\n" \
    "            \"name\": \"anotherprinterservice._printer-s-ws-https._tcp.srv.arces.unibo.it.\",\n" \
    "            \"type\": \"_printer-s-ws-https._tcp\",\n" \
    "            \"domain\": \"168.56.101.\",\n" \
    "            \"host\": \"192.168.56.101.\",\n" \
    "            \"port\": 8055,\n" \
    "            \"properties\": {\n" \
    "                \"property\": [\n" \
    "                    {\n" \
    "                        \"name\": \"version\",\n" \
    "                        \"value\": \"1.0\"\n" \
    "                    },\n" \
    "                    {\n" \
    "                        \"name\": \"path\",\n" \
    "                        \"value\": \"/printer/something\"\n" \
    "                    }\n" \
    "                ]\n" \
    "            }\n" \
    "        }\n" \
    "    ]\n" \
    "}\n"

#define TEST_JSON_LIST_EMPTY_TEXT "{\"service\": []}"

#define TEST_NOT_JSON_TEXT "[]; []{ } <xml> blah"


SCENARIO( "Services are parsed from JSON", "[servicejson]" ) {

    GIVEN("an empty destination vector") {
        std::vector<Arrowhead::ServiceDescription> servicelist;
        REQUIRE(servicelist.empty());

        WHEN("an empty JSON service list string is parsed" ) {
            std::string js(TEST_JSON_LIST_EMPTY_TEXT);
            Arrowhead::parse_servicelist_json(std::back_inserter(servicelist), js);
            THEN("the vector is still empty") {
                REQUIRE(servicelist.empty());
            }
        }
        WHEN("an JSON service list string containing 3 services is parsed" ) {
            std::string js(TEST_JSON_LIST_2_SERVICES_TEXT);
            Arrowhead::parse_servicelist_json(std::back_inserter(servicelist), js);
            THEN("the vector is extended with the supplied services") {
                REQUIRE(servicelist.size() == 2);
                REQUIRE(servicelist[0].name == "orchestration-store._orch-s-ws-https._tcp.srv.arces.unibo.it.");
                REQUIRE(servicelist[0].type == "_orch-s-ws-https._tcp");
                REQUIRE(servicelist[0].host == "bedework.arces.unibo.it.");
                REQUIRE(servicelist[0].domain == "arces.unibo.it.");
                REQUIRE(servicelist[0].port == 8181);
                REQUIRE(servicelist[0].properties["version"] == "1.1");
                REQUIRE(servicelist[0].properties["path"] == "/orchestration/store/");
                REQUIRE(servicelist[1].name == "anotherprinterservice._printer-s-ws-https._tcp.srv.arces.unibo.it.");
                REQUIRE(servicelist[1].type == "_printer-s-ws-https._tcp");
                REQUIRE(servicelist[1].host == "192.168.56.101.");
                REQUIRE(servicelist[1].domain == "168.56.101.");
                REQUIRE(servicelist[1].port == 8055);
                REQUIRE(servicelist[1].properties["version"] == "1.0");
                REQUIRE(servicelist[1].properties["path"] == "/printer/something");
            }
        }
        WHEN("a non-JSON string is passed to the JSON parser") {
            std::string js(TEST_NOT_JSON_TEXT);
            REQUIRE_THROWS(Arrowhead::parse_servicelist_json(std::back_inserter(servicelist), js));
            THEN("the vector is still empty") {
                REQUIRE(servicelist.empty());
            }
        }
    }
}
