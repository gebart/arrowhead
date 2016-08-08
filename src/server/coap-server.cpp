#include "arrowhead/config.h"

#include <boost/asio.hpp>

#include "coap/coap.h"
#include "arrowhead/coap.hpp"

/*
 * The resource handler
 */
static void
hello_handler(coap_context_t *ctx, struct coap_resource_t *resource,
              const coap_endpoint_t *local_interface, coap_address_t *peer,
              coap_pdu_t *request, str *token, coap_pdu_t *response)
{
    (void)ctx;
    (void)resource;
    unsigned char buf[3];
    const char *response_data = "Hello World!";
    response->hdr->code       = COAP_RESPONSE_CODE(205);
    coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
    coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);
}

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;

    Arrowhead::CoAPResource hello("hello");
    hello.register_handler(COAP_REQUEST_GET, hello_handler);

    Arrowhead::CoAPContext server(io_service, 13131);
    server.add_resource(hello);

    io_service.run();
    return 0;
}
