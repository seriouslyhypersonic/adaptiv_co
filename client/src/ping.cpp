/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <stdexcept>
#include <sstream>
#include <iomanip>

#include <adaptiv/net/net.hpp>
#include <adaptiv/cloud/protocol/response.hpp>

#include <adaptiv/definitions.hpp>

#include "ping.hpp"

namespace net = adaptiv::net;

client::protocol::responses::ServerStatus
client::rpc::ping(beast::websocket::stream<beast::tcp_stream>& websocket
)
{
    beast::error_code ec;

    // This buffer will hold the incoming message
    beast::flat_buffer buffer;

    // Read a message into our buffer
    websocket.read(buffer, ec);
    if(ec) adaptiv::except(ec, "read");

    auto in = beast::buffers_to_string(buffer.data());
    protocol::Response<protocol::responses::ServerStatus> response(in);

    return response.message();
}

client::protocol::responses::ServerStatus
client::rpc::ping(std::string const& host, std::string const& port)
{
    adaptiv::error_code ec;

    // The io_context is required for all I/O
    net::io_context context;

    // These objects perform our I/O
    net::tcp::resolver resolver{context};
    beast::websocket::stream<beast::tcp_stream> ws{context};

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

    auto status = ping(ws);

//    // Close the WebSocket connection
//    ws.close(beast::websocket::close_code::normal);
//    if(ec) adaptiv::except(ec, "close");

    return status;
}

void client::ping(std::string const& host, std::string const& port)
{
    try {
        auto status = rpc::ping(host, port);
        auto largew = std::setw(2*adaptiv::def::output::fieldw);
        auto hline = std::string(2*adaptiv::def::output::fieldw, '-');
        std::cout <<
            hline << '\n' <<
            largew <<
                std::left << "Pinging " + host + ':' + port <<'\n' <<
            hline << '\n' <<
            status << '\n' <<
            largew << hline << '\n';
    } catch (std::exception const& exception) {
        adaptiv::fail(exception.what());
    }
}
