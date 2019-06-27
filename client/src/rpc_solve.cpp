/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <sstream>
#include <cstddef>
#include <stdexcept>

#include <adaptiv/net/net.hpp>
#include <adaptiv/cloud/protocol/response.hpp>
#include <adaptiv/cloud/protocol/messages/rans.hpp>

#include "rpc_solve.hpp"

namespace net = adaptiv::net;
namespace beast = adaptiv::beast;
namespace protocol = adaptiv::cloud::protocol;

// Output the result with headers
void printIteration(
    protocol::RANSResponse const& response,
    std::size_t headerSpacing)
{
    if ((headerSpacing != 0 &&
        response.iteration % headerSpacing - 1 == 0) || // Use header spacing
        response.iteration == 1) {                      // Print for first iter
        std::cout << protocol::RANSResponse::header();
    }
    std::cout << response;
}

/// Call target solve on the server
void client::rpcSolve(
    std::string const& host,
    std::string const& port)
{
    beast::error_code ec;

    // The io_context is required for all I/O
    net::io_context context;

    // These objects perform our I/O
    net::tcp::resolver resolver(context);
    beast::websocket::stream<beast::tcp_stream> ws(context);

    // Look up the domain name
    auto const results = resolver.resolve(host, port, ec);
    if(ec) adaptiv::except(ec, "resolve");

    // Set a timeout on the operation
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws).connect(results, ec);
    if(ec) adaptiv::except(ec, "connect");

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(ws).expires_never();

    // Set suggested timeout settings for the websocket
    ws.set_option(
        beast::websocket::stream_base::timeout::suggested(
            beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    ws.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req)
        {
            req.set(beast::http::field::user_agent,
                    std::string(ADAPTIV_VERSION_STRING) +
                    "-client-coro");
        }));


    // Perform the websocket handshake
    ws.handshake(host, "/", ec);
    if(ec) adaptiv::except(ec, "handshake");

    // Send the message // todo: implement solve request using NetworkExchange
    std::string solveRequest = "solve";
    ws.write(net::buffer(solveRequest), ec);
    if(ec) adaptiv::except(ec, "write");

    // This buffer will hold the incoming message
    beast::flat_buffer buffer;

    // Read a message into our buffer
    ws.read(buffer, ec);
    if(ec) adaptiv::except(ec, "read");

    // Print welcome message
    std::cout << beast::make_printable(buffer.data()) << std::endl;

    bool busy = false;
    do {
        // Read rans response
        beast::flat_buffer msgBuffer;
        ws.read(msgBuffer, ec);
        if (ec) adaptiv::except(ec, "read response");

        std::istringstream in(beast::buffers_to_string(msgBuffer.data()));

        protocol::Response<protocol::RANSResponse> response(in);
        printIteration(response.message(), 5);

        busy = response.message().busy;
    } while (busy);


    // Close the WebSocket connection
    ws.close(beast::websocket::close_code::normal, ec);
    if(ec) adaptiv::except(ec, "close");

    // If we get here then the connection is closed gracefully
}

void client::doSolve(
    std::string const& host,
    std::string const& port)
{
    try {
        rpcSolve(host, port);
    } catch (std::exception const& exception) {
        adaptiv::fail(exception.what());
    }

}
