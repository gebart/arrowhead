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
 * @brief       Arrowhead query command-line client application implementation
 *
 * @author      Joakim Gebart Nohlgård <joakim@nohlgard.se>
 *
 */
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <list>

#include <boost/program_options.hpp>

#include "arrowhead/logging.hpp"
#include "arrowhead/service.hpp"
#include "arrowhead/xml.hpp"
#include "arrowhead/http.hpp"
#include "arrowhead/core_services/serviceregistry.hpp"
#include "project_version.h"

namespace po = boost::program_options;

using namespace Arrowhead;

/**
 * @ingroup  cli
 *
 * @{
 */

/**
 * @ingroup  cli
 * @brief    Main application class for Service Registry command line interface
 */
class ArrowheadQueryApp {
    private:
        static const std::string version;

        po::variables_map options;

    public:
        /**
         * @brief  Initialize the application
         *
         * @param[in]  argc Command line argument count
         * @param[in]  argv Command line arguments
         *
         * @return 0 on success
         * @return non-zero if something went wrong
         */
        int init(int argc, char * argv[]);

        /**
         * @brief  Perform the actions given on the command line
         *
         * @return 1 if the action was not specified
         * @return 0 if the action was performed
         */
        int run();

        /**
         * @brief  Query service registry and print results to standard output
         *
         * If provided, the first argument of args will be used as a service
         * type string to filter the search by.
         *
         * @param[in]  args  arguments passed from command line
         */
        void list(std::list<std::string> args);

        /**
         * @brief  Publish a service in the service registry
         *
         * @todo Implement argument parsing
         *
         * @param[in]  args  arguments passed from command line
         */
        void publish(std::list<std::string> args);

        /**
         * @brief  Unpublish (delete) a service in the service registry
         *
         * Each argument in args will be used as a service name and will be
         * unpublished in the given order.
         *
         * @todo Test against server
         *
         * @param[in]  args  arguments passed from command line
         */
        void unpublish(std::list<std::string> args);
};

int ArrowheadQueryApp::init(int argc, char * argv[])
{
    // Initialize libcurl globals first of all, this function must only be
    // called once and is not thread safe and may not be called when any other
    // threads are running
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize default logging
    Logging::configure();
    ARROWHEAD_LIB_LOGGER(logger, "ArrowheadQueryApp::init");

    // Declare a group of options that will be
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "this help message")
        ("config,c", po::value<std::string>()->default_value("arrowhead.cfg"),
              "configuration file")
        ("action,a", po::value<std::string>()->default_value("list"),
              "action (list, publish, unpublish)")
        ;

    // Declare a group of options that will be
    // allowed both on command line and in
    // config file
    po::options_description config("Configuration");
    config.add_options()
        ("url,u",
            po::value<std::string>()->
            default_value("http://ns.arrowhead.ltu.se:8045/servicediscovery"),
            "REST API URL base")
        ("logconf",
            po::value<std::string>()->
            default_value("log4cplus.properties"),
            "logging configuration file")
        ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("action_args", po::value< std::vector<std::string> >(), "arguments passed to the action")
        ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("action", 1);
    p.add("action_args", -1);

    try {
        po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).positional(p).run(), options);
        po::notify(options);
    }
    catch (po::error& e) {
        ARROWHEAD_LIB_ERROR(logger, e.what());
        return 1;
    }

    if (options.count("help")) {
        std::cout << "Usage: " << argv[0] << " [options] action [args]" << std::endl;
        std::cout << "Arrowhead service registry REST tool" << std::endl;
        std::cout << visible << std::endl;
        return 1;
    }

    if (options.count("version")) {
        std::cout << "Arrowhead service registry REST tool version " VCS_VERSION_STR << std::endl;
        return 1;
    }

    try {
        std::string config_file = options["config"].as<std::string>();
        std::ifstream ifs(config_file.c_str());
        if (ifs.good()) {
            po::store(parse_config_file(ifs, config_file_options), options);
            po::notify(options);
        }
        else {
            ARROWHEAD_LIB_WARN(logger, "Failed to open config file '" <<
                config_file << "' for reading. (ignored)");
        }
    }
    catch (po::error& e) {
        ARROWHEAD_LIB_ERROR(logger, e.what());
        return 1;
    }

    Logging::configure(options["logconf"].as<std::string>());
    return 0;
}

int ArrowheadQueryApp::run()
{
    ARROWHEAD_LIB_LOGGER(logger, "ahq::run");
    ARROWHEAD_LIB_TRACE(logger, "+ArrowheadQueryApp::run");

    ARROWHEAD_LIB_INFO(logger, "Starting (version: " VCS_VERSION_STR ")...");

    ARROWHEAD_LIB_INFO(logger, "Using Service Directory at " << options["url"].as<std::string>());

    std::string action = options["action"].as<std::string>();

    std::list<std::string> args;

    if (options.count("action_args")) {
        const std::vector<std::string>& tmp = options["action_args"].as<const std::vector<std::string> >();
        std::copy(tmp.begin(), tmp.end(), std::back_inserter(args));
    }

    if (action == "list") {
        list(args);
    }
    else if (action == "publish") {
        publish(args);
    }
    else if (action == "unpublish") {
        unpublish(args);
    }
    else {
        ARROWHEAD_LIB_FATAL(logger, "Unknown action '" << action << "'");
        return 1;
    }
    ARROWHEAD_LIB_TRACE(logger, "-ArrowheadQueryApp::run");
    return 0;
}

void ArrowheadQueryApp::list(std::list<std::string> args)
{
    ARROWHEAD_LIB_LOGGER(logger, "ahq::list");
    ARROWHEAD_LIB_TRACE(logger, "+ArrowheadQueryApp::list");
    ServiceRegistryHTTP servicereg(options["url"].as<std::string>());
    std::string type;

    if (!args.empty()) {
        type = args.front();
        args.pop_front();
    }
    std::string xml = servicereg.list(type);
    std::vector<ServiceDescription> servicelist;
    parse_servicelist_xml(std::back_inserter(servicelist), xml);

    ARROWHEAD_LIB_INFO(logger, servicelist.size() << " services:");
    for (auto& srv: servicelist) {
        ARROWHEAD_LIB_INFO(logger, " - " << srv.name);
    }

    ARROWHEAD_LIB_DEBUG(logger, "Detailed list:");
    for (auto& srv: servicelist) {
        ARROWHEAD_LIB_DEBUG(logger, srv.name << ":");
        ARROWHEAD_LIB_DEBUG(logger, "  type: " << srv.type);
        ARROWHEAD_LIB_DEBUG(logger, "  domain: " << srv.domain);
        ARROWHEAD_LIB_DEBUG(logger, "  host: " << srv.host);
        ARROWHEAD_LIB_DEBUG(logger, "  port: " << srv.port);
        ARROWHEAD_LIB_DEBUG(logger, "  properties:");
        for (auto& kv: srv.properties) {
            ARROWHEAD_LIB_DEBUG(logger, "    " << kv.first << ": " << kv.second);
        }
    }
    ARROWHEAD_LIB_TRACE(logger, "-ArrowheadQueryApp::list");
}

void ArrowheadQueryApp::publish(std::list<std::string> args)
{
    ARROWHEAD_LIB_LOGGER(logger, "ahq::publish");
    ARROWHEAD_LIB_TRACE(logger, "+ArrowheadQueryApp::publish");
    ServiceRegistryHTTP servicereg(options["url"].as<std::string>());

    if (args.empty()) {
        throw(std::runtime_error("ahq::publish Missing name"));
    }
    ServiceDescription srv;
    srv.name = args.front();
    args.pop_front();
    srv.type = "_rpm-coap._udp";
    srv.domain = "arrowhead.localtest.";
    srv.host = "ahq.example.com.";
    srv.port = 12345;
    srv.properties["path"] = std::string("/rpm");
    srv.properties["version"] = std::string("0.1");

    servicereg.publish(srv);
    ARROWHEAD_LIB_TRACE(logger, "-ArrowheadQueryApp::publish");
}

void ArrowheadQueryApp::unpublish(std::list<std::string> args)
{
    ARROWHEAD_LIB_LOGGER(logger, "ahq::unpublish");
    ARROWHEAD_LIB_TRACE(logger, "+ArrowheadQueryApp::unpublish");
    ServiceRegistryHTTP servicereg(options["url"].as<std::string>());
    if (args.empty()) {
        throw(std::runtime_error("ahq::unpublish Missing name"));
    }
    std::string name = args.front();
    args.pop_front();

    servicereg.unpublish(name);
    ARROWHEAD_LIB_TRACE(logger, "-ArrowheadQueryApp::unpublish");
}

int main(int argc, char * argv[])
{
    ArrowheadQueryApp app;
    int res;
    res = app.init(argc, argv);
    if (res != 0) {
        return res;
    }
    res = app.run();
    return res;
}

/**
 * @}
 */
