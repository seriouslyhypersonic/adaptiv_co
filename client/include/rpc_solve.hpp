/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_CORO_RPC_SOLVE_H
#define ADAPTIV_CORO_RPC_SOLVE_H

#include <string>

#include <adaptiv/net/net.hpp>
#include <adaptiv/cloud/protocol/response.hpp>
#include <adaptiv/cloud/protocol/messages/rans.hpp>

namespace client {

namespace net = adaptiv::net;
namespace beast = adaptiv::beast;

namespace detail {
/// Call target solve on the server
void rpcSolve(
        std::string const& host,
        std::string const& port,
        net::io_context& context,
        net::yield_context yield)
{
    adaptiv::error_code ec;

    // These objects perform our I/O
    net::tcp::resolver resolver(context);
    beast::websocket::stream<beast::tcp_stream> ws(context);

    // Look up the domain name
    auto const results = resolver.async_resolve(host, port, yield[ec]);
    if(ec) return adaptiv::fail(ec, "resolve");

    // Set a timeout on the operation
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws).async_connect(results, yield[ec]);
    if(ec) return adaptiv::fail(ec, "connect");

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
    ws.async_handshake(host, "/", yield[ec]);
    if(ec) return adaptiv::fail(ec, "handshake");

    // Send the message
    std::string solveRequest = "solve";
    ws.async_write(net::buffer(solveRequest), yield[ec]);
    if(ec) return adaptiv::fail(ec, "write");

    // This buffer will hold the incoming message
    beast::flat_buffer buffer;

    // Read a message into our buffer
    ws.async_read(buffer, yield[ec]);
    if(ec) return adaptiv::fail(ec, "read");

    // Print welcome message
    std::cout << beast::make_printable(buffer.data()) << std::endl;

    bool busy = false;
    do {
    } while (busy);



    // Close the WebSocket connection
    ws.async_close(beast::websocket::close_code::normal, yield[ec]);
    if(ec) return adaptiv::fail(ec, "close");

    // If we get here then the connection is closed gracefully
}
} // namespace detail

void doSolve(
    std::string const& host,
    std::string const& port)
{
    // The io_context is required for all I/O
    net::io_context context;

    // Launch the asynchronous operation
    net::spawn(
        context,
        [&context, &host, &port](net::yield_context yield)
        {
            detail::rpcSolve(host, port, context, yield);
        });

    context.run();
}

} // namespace client

#endif //ADAPTIV_CORO_RPC_SOLVE_H
