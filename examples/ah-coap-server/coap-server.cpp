#include "arrowhead/config.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

#include "coap/coap.h"
#include "arrowhead/coap.hpp"
#include "arrowhead/core_services/serviceregistry.hpp"

/* Ugly global state because of libcoap limitations (no opaque pointer in base
 * coap context struct) */
int count = 0;

/*
 * The resource handler
 */
static void hello_handler(coap_context_t *ctx, struct coap_resource_t *resource,
              const coap_endpoint_t *local_interface, coap_address_t *peer,
              coap_pdu_t *request, str *token, coap_pdu_t *response)
{
    (void)ctx;
    (void)resource;
    unsigned char buf[3];
    response->hdr->code = COAP_RESPONSE_CODE(205);
    coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

    std::ostringstream os;
    os << count << " seconds since start";
    std::string str(os.str());
    // coap_add_data must be the last operation on the PDU.
    coap_add_data(response, str.length(), reinterpret_cast<const unsigned char *>(str.c_str()));
}

void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* tim, int* count)
{
    std::cout << *count << std::endl;
    ++(*count);

    tim->expires_at(tim->expires_at() + boost::posix_time::seconds(1));
    tim->async_wait(boost::bind(print,
        boost::asio::placeholders::error, tim, count));
}

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;

    Arrowhead::CoAPResource hello("hello");
    hello.register_handler(COAP_REQUEST_GET, hello_handler);

    Arrowhead::CoAPContext server(io_service, 13131);
    server.add_resource(hello);

    boost::asio::deadline_timer tim(io_service, boost::posix_time::seconds(1));
    tim.async_wait(boost::bind(print,
        boost::asio::placeholders::error, &tim, &count));

    Arrowhead::ServiceRegistryHTTP servicereg("http://localhost:8045/servicediscovery");
    Arrowhead::ServiceDescription srv;
    srv.name = argv[0];
    srv.type = "example-server.arrowhead.cpp";
    srv.host = "localhost";
    srv.port = 13131;
    srv.properties["version"] = "0.1";
    srv.properties["path"] = "/hello";
    servicereg.publish(srv);

    io_service.run();
    return 0;
}
