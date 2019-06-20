/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef WEBSOCKET_SESSION_HPP
#define WEBSOCKET_SESSION_HPP

#include <memory>
#include <queue>

#include <adaptiv/cloud/cloud.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

class SharedState;

/// Represents an active WebSocket connection
class WebSocketSession
{
    /// The underlying I/O object for current session
    beast::websocket::stream<beast::tcp_stream> websocket_;

    /// The server state, accessible from all active sessions
    std::shared_ptr<SharedState> state_;

    /// A per-session queue of outgoing messages
    std::queue<std::shared_ptr<std::string const>> queue_;

    friend class SharedState;

public:
    explicit WebSocketSession(
        net::tcp::socket&& socket,
        std::shared_ptr<SharedState> state);

    ~WebSocketSession();

    /// Accept the websocket handshake
    template<class Body, class Allocator>
    error_code accept(
        beast::http::request<Body
                            ,beast::http::basic_fields<Allocator>> request,
        net::yield_context yield);

    /// Write queue to the websocket
    void write(net::yield_context yield);

    /// Read a message from the websocket and put it into the queue
    void read(bool& hasConnection, net::yield_context yield);

    /// Convenience function - make a shared_ptr
    static std::shared_ptr<WebSocketSession> makeShared(
        net::tcp::socket&& socket,
        std::shared_ptr<SharedState> const& state);
};

template<class Body, class Allocator>
error_code WebSocketSession::accept(
    beast::http::request<Body
                        ,beast::http::basic_fields<Allocator>> request,
    net::yield_context yield)
{
    error_code ec;

    // Set suggested timeout for the WebSocket
    websocket_.set_option(
        beast::websocket::stream_base::timeout::suggested(
            beast::role_type::server));

    // Set a decorator to change the server of the handshake
    websocket_.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::response_type& response)
        {
            response.set(beast::http::field::server,
                         std::string(ADAPTIV_VERSION_STRING) + "_server");
        }));

    // Accept the Websocket handshake
    websocket_.async_accept(request, yield[ec]);
    return ec;
}

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //WEBSOCKET_SESSION_HPP
