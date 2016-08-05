#include <netinet/in.h> // For sockaddr_in6 et. al
#include <stdexcept>
#include <new>
#include <iostream>

#include "arrowhead/config.h"
#include "coap/coap.h"

class CoAPResource {
    public:
        CoAPResource(const char *uri) : _uri(uri)
        {
            const unsigned char *urip = reinterpret_cast<const unsigned char *>(_uri.c_str());
            int flags = 0;
            res = coap_resource_init(urip, _uri.length(), flags);
            if (res == 0) {
                // Failed to allocate memory for resource descriptor
                throw std::bad_alloc();
            }
        }

        template<class StringType>
            CoAPResource(const StringType& uri) : _uri(uri)
        {
            const unsigned char *urip = reinterpret_cast<const unsigned char *>(_uri.c_str());
            int flags = 0;
            res = coap_resource_init(urip, _uri.length(), flags);
            if (res == 0) {
                // Failed to allocate memory for resource descriptor
                throw std::bad_alloc();
            }
        }

        // Disable copying for now
        CoAPResource(CoAPResource const&) = delete;
        CoAPResource& operator=(CoAPResource const&) = delete;

        void register_handler(unsigned char method, coap_method_handler_t handler)
        {
            coap_register_handler(res, method, handler);
        }

        coap_resource_t *resource_ptr(void)
        {
            return res;
        }

    private:
        coap_resource_t *res;
        std::string _uri;
};

class CoAPServer {
    public:
        CoAPServer(unsigned short port = 5683)
        {
            coap_address_init(&listen_addr);
            listen_addr.addr.sin6.sin6_family = AF_INET6;
            listen_addr.addr.sin6.sin6_addr   = IN6ADDR_ANY_INIT;
            listen_addr.addr.sin6.sin6_port   = htons(port);
            ctx = coap_new_context(&listen_addr);
            if (ctx == 0) {
                // Failed to allocate a new context struct
                throw std::bad_alloc();
            }
        }

        CoAPServer(const struct sockaddr_in6& sin6)
        {
            coap_address_init(&listen_addr);
            listen_addr.addr.sin6 = sin6;
            ctx = coap_new_context(&listen_addr);
            if (ctx == 0) {
                // Failed to allocate a new context struct
                throw std::bad_alloc();
            }
        }

        ~CoAPServer()
        {
            // Clean up context
            std::cerr << "destroying context" << std::endl;
            coap_free_context(ctx);
        }

        // Disable copying for now
        CoAPServer(CoAPServer const&) = delete;
        CoAPServer& operator=(CoAPServer const&) = delete;

        void add_resource(coap_resource_t *res)
        {
            coap_add_resource(ctx, res);
        }

        void add_resource(CoAPResource& res)
        {
            coap_add_resource(ctx, res.resource_ptr());
        }

        void run_forever(void)
        {
            using namespace std;
            fd_set readfds;
            /*Listen for incoming connections*/
            while (1) {
                FD_ZERO(&readfds);
                FD_SET(ctx->sockfd, &readfds);
                int result = select(FD_SETSIZE, &readfds, 0, 0, 0);
                if (result < 0) /* socket error */
                {
                    throw std::runtime_error("socket read error");
                }
                else if (result > 0 && FD_ISSET(ctx->sockfd, &readfds))
                {
                    coap_read(ctx);
                }
            }
        }

        void run_once(void)
        {
            using namespace std;
            fd_set readfds;
            /*Listen for incoming connections*/
            FD_ZERO(&readfds);
            FD_SET(ctx->sockfd, &readfds);
            int result = select(FD_SETSIZE, &readfds, 0, 0, 0);
            if (result < 0) /* socket error */
            {
                throw std::runtime_error("socket read error");
            }
            else if (result > 0 && FD_ISSET(ctx->sockfd, &readfds))
            {
                coap_read(ctx);
            }
        }
    private:
        coap_context_t *ctx;
        coap_address_t  listen_addr;

};

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
    const char* response_data     = "Hello World!";
    response->hdr->code           = COAP_RESPONSE_CODE(205);
    coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
    coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);
}

int main(int argc, char* argv[])
{
    CoAPServer server(13131);
    CoAPResource hello("hello");
    hello.register_handler(COAP_REQUEST_GET, hello_handler);
    server.add_resource(hello);
    server.run_forever();
    return 0;
}
