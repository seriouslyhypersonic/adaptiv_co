/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "websocket_session.hpp"
#include "shared_state.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

WebSocketSession::WebSocketSession(
    net::tcp::socket&& socket,
    std::shared_ptr<SharedState> state)
    : websocket_(std::move(socket))
    , state_(std::move(state))
{
    state_->join(this);
}

WebSocketSession::~WebSocketSession()
{
    state_->leave(this);
}

void WebSocketSession::write(boost::asio::yield_context yield)
{
    ADAPTIV_DEBUG_CERR("->write(id:" << this << ')');
    error_code ec;

    while (!queue_.empty()) {
        // Send the message
        websocket_.async_write(net::buffer(*queue_.front()), yield[ec]);
        if (ec) return fail(ec, "websocket write");

        // Remove what we just sent
        queue_.pop();
    }
    ADAPTIV_DEBUG_CERR("<-write(id:" << this << ')');
}

void WebSocketSession::read(bool& hasConnection,
                            boost::asio::yield_context yield)
{
    ADAPTIV_DEBUG_CERR("->read(id:" << this << ')');

    error_code ec;

    // This buffer will hold the incoming message
    beast::flat_buffer buffer;

    websocket_.async_read(buffer, yield[ec]);
    ADAPTIV_DEBUG_CERR(this << ": " << beast::buffers_to_string(buffer.data()));

    // This indicates that the session was closed
    if (ec == beast::websocket::error::closed) {
        hasConnection = false;
        return;
    }

    if (ec) {
        hasConnection = false;
        return fail(ec, "websocket read");
    }

    // Push message to all queues
    auto message = beast::buffers_to_string(buffer.data());
    state_->pushToAll(message);

    ADAPTIV_DEBUG_CERR("<-read(id:" << this << ')');
}

std::shared_ptr<WebSocketSession> WebSocketSession::makeShared(
    net::ip::tcp::socket&& socket,
    std::shared_ptr<SharedState> const& state)
{
    return std::make_shared<WebSocketSession>(std::move(socket), state);
}

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
