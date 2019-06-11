#include <iostream>
#include <memory>
#include <cstdlib>

#include <adaptiv/net/net.hpp>

#include "server.hpp"

namespace server = adaptiv::cloud::server;
namespace net = adaptiv::net;

int main(int argc, char** argv)
{
    // Check command line arguments
    if (argc != 4) {
        std::cerr <<
                  "  Usage: server <address> <port> <documentRoot>\n" <<
                  "Example:\n" <<
                  "         server 127.0.0.1 8080 .\n";
        return EXIT_FAILURE;
    }
    auto const address = adaptiv::net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const documentRoot = argv[3];

    // The io_context is required for all I/O
    net::io_context context;

    // Spawn a listening port
    net::spawn(
        context,
        [&context, address, port, documentRoot]
        (net::yield_context yield)
        {
            server::doListen(
                context,
                net::tcp::endpoint{address, port},
                server::SharedState::makeShared(documentRoot),
                yield);
        }
    );

    context.run();

    return EXIT_SUCCESS;
}