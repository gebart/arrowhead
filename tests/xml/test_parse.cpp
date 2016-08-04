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
#include <vector>
#include <iterator>

// Example XML data taken from the Arrowhead document repository.
#define TEST_XML_LIST_3_SERVICES_TEXT "" \
"<serviceList>\n" \
"    <service>\n" \
"        <domain>168.56.101.</domain>\n" \
"        <host>192.168.56.101.</host>\n" \
"        <name>anotherprinterservice._printer-s-ws-https._tcp.srv.arces.unibo.it.</name>\n" \
"        <port>8055</port>\n" \
"        <properties>\n" \
"            <property>\n" \
"                <name>version</name>\n" \
"                <value>1.0</value>\n" \
"            </property>\n" \
"            <property>\n" \
"                <name>path</name>\n" \
"                <value>/printer/something</value>\n" \
"            </property>\n" \
"        </properties>\n" \
"        <type>_printer-s-ws-https._tcp</type>\n" \
"    </service>\n" \
"    <service>\n" \
"        <domain>arces.unibo.it.</domain>\n" \
"        <host>bedework.arces.unibo.it.</host>\n" \
"        <name>authorisation-ctrl._auth-ws-https._tcp.srv.arces.unibo.it.</name>\n" \
"        <port>8181</port>\n" \
"        <properties>\n" \
"            <property>\n" \
"                <name>version</name>\n" \
"                <value>0.2</value>\n" \
"            </property>\n" \
"            <property>\n" \
"                <name>path</name>\n" \
"                <value>/authorisation-control/</value>\n" \
"            </property>\n" \
"        </properties>\n" \
"        <type>_auth-ws-https._tcp</type>\n" \
"    </service>\n" \
"    <service>\n" \
"        <domain>arces.unibo.it.</domain>\n" \
"        <host>bedework.arces.unibo.it.</host>\n" \
"        <name>orchestration-store._orch-s-ws-https._tcp.srv.arces.unibo.it.</name>\n" \
"        <port>8181</port>\n" \
"        <properties>\n" \
"            <property>\n" \
"                <name>version</name>\n" \
"                <value>1.1</value>\n" \
"            </property>\n" \
"            <property>\n" \
"                <name>path</name>\n" \
"                <value>/orchestration/store/</value>\n" \
"            </property>\n" \
"        </properties>\n" \
"        <type>_orch-s-ws-https._tcp</type>\n" \
"    </service>\n" \
"</serviceList>\n" \
""

#define TEST_XML_LIST_EMPTY_TEXT "<serviceList></serviceList>"

#define TEST_NOT_XML_TEXT \
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
    "                        \"value\": \"1.0\"\n" \
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


SCENARIO( "Services are parsed from XML", "[servicexml]" ) {

    GIVEN("an empty destination vector") {
        std::vector<Arrowhead::ServiceDescription> servicelist;
        REQUIRE(servicelist.empty());

        WHEN("an empty XML service list string is parsed" ) {
            std::string xml(TEST_XML_LIST_EMPTY_TEXT);
            Arrowhead::parse_servicelist_xml(std::back_inserter(servicelist), xml);
            THEN("the vector is still empty") {
                REQUIRE(servicelist.empty());
            }
        }
        WHEN("an XML service list string containing 3 services is parsed" ) {
            std::string xml(TEST_XML_LIST_3_SERVICES_TEXT);
            Arrowhead::parse_servicelist_xml(std::back_inserter(servicelist), xml);
            THEN("the vector is extended with the supplied services") {
                REQUIRE(servicelist.size() == 3);
                REQUIRE(servicelist[0].name == "anotherprinterservice._printer-s-ws-https._tcp.srv.arces.unibo.it.");
                REQUIRE(servicelist[0].type == "_printer-s-ws-https._tcp");
                REQUIRE(servicelist[0].host == "192.168.56.101.");
                REQUIRE(servicelist[0].domain == "168.56.101.");
                REQUIRE(servicelist[0].port == 8055);
                REQUIRE(servicelist[0].properties["version"] == "1.0");
                REQUIRE(servicelist[0].properties["path"] == "/printer/something");
                REQUIRE(servicelist[1].name == "authorisation-ctrl._auth-ws-https._tcp.srv.arces.unibo.it.");
                REQUIRE(servicelist[1].type == "_auth-ws-https._tcp");
                REQUIRE(servicelist[1].host == "bedework.arces.unibo.it.");
                REQUIRE(servicelist[1].domain == "arces.unibo.it.");
                REQUIRE(servicelist[1].port == 8181);
                REQUIRE(servicelist[1].properties["version"] == "0.2");
                REQUIRE(servicelist[1].properties["path"] == "/authorisation-control/");
            }
        }
        WHEN("a non-xml string is passed to the XML parser") {
            std::string xml(TEST_NOT_XML_TEXT);
            REQUIRE_THROWS_AS(Arrowhead::parse_servicelist_xml(std::back_inserter(servicelist), xml), Arrowhead::ContentError);
            THEN("the vector is still empty") {
                REQUIRE(servicelist.empty());
            }
        }
    }
}
