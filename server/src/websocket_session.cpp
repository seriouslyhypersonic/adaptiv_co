/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "websocket_session.hpp"
#include "shared_state.hpp"

// WIP
#include <string>
#include <sstream>
#include <algorithm>

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

void WebSocketSession::message(
    std::string const& msg,
    boost::asio::yield_context yield)
{
    error_code ec;
    websocket_.async_write(net::buffer(msg), yield[ec]);
    if (ec) return fail(ec, "websocket welcome");
}

bool WebSocketSession::queueEmpty()
{
    std::lock_guard guard(state_->mutex());
    return queue_.empty();
}

std::shared_ptr<std::string const>& WebSocketSession::queueFront()
{
    std::lock_guard guard(state_->mutex());
    return queue_.front();
}

void WebSocketSession::queuePop()
{
    std::lock_guard guard(state_->mutex());
    queue_.pop();
}

void WebSocketSession::queueClear()
{
    std::lock_guard guard(state_->mutex());
    std::queue<std::shared_ptr<std::string const>> empty;
    std::swap(queue_, empty);
}

std::queue<std::shared_ptr<std::string const>> WebSocketSession::queueCopy()
{
    std::queue<std::shared_ptr<std::string const>>  tempQueue;
    {
        std::lock_guard guard(state_->mutex());
        tempQueue = queue_;
    }
    return tempQueue;
}

void WebSocketSession::welcome(boost::asio::yield_context yield)
{
    std::stringstream msg;
    msg << "{busy:" << std::boolalpha << state_->isBusy() << "}";

    message(msg.str(), yield);
}

void WebSocketSession::write(boost::asio::yield_context yield)
{
    ADAPTIV_DEBUG_CERR("->write(id:" << this << ')');

    error_code ec;

    while (!queueEmpty()) {
        // Send the message
        auto msg = *queueFront();
        websocket_.async_write(net::buffer(msg), yield[ec]);
        if (ec) return fail(ec, "websocket write");

        // Remove what we just sent
        queuePop();
    }
    ADAPTIV_DEBUG_CERR("<-write(id:" << this << ')');
}

void WebSocketSession::flush(boost::asio::yield_context yield)
{
    ADAPTIV_DEBUG_CERR("->flush(id:" << this << ')');
    isFlushing_ = true;

    // Copy and clear - freeing queue_ before yielding;
    auto tempQueue = queueCopy();
    queueClear();

    error_code ec;
    while (!tempQueue.empty()) {
        websocket_.async_write(net::buffer(*tempQueue.front()), yield[ec]);
        if (ec) return fail(ec, "websocket flush");

        // Remove what we just sent
        tempQueue.pop();
    }

    isFlushing_ = false;
    ADAPTIV_DEBUG_CERR("<-flush(id:" << this << ')');
}

void WebSocketSession::run(boost::asio::yield_context yield)
{
    ADAPTIV_DEBUG_CERR("->run(id:" << this << ')');

    error_code ec;

    // This buffer will hold the incoming message
    beast::flat_buffer buffer;

    // Only run a request if we're not busy
    if (!state_->isBusy()) {
        websocket_.async_read(buffer, yield[ec]);
        ADAPTIV_DEBUG_CERR(this << ":" << beast::buffers_to_string(buffer.data()));
    }

    // This indicates that the session was closed
    if (ec == beast::websocket::error::closed) {
        hasConnection_ = false;
        return;
    }

    if (ec) {
        hasConnection_ = false;
        return fail(ec, "websocket run");
    }

    // Process the request
    if (!state_->isBusy()) {
        auto command = beast::buffers_to_string(buffer.data());
        if (command == "solve") {
            state_->solve();
            ADAPTIV_DEBUG_CERR("starting solver...");
        } else {
            message("{result:invalid}", yield);
        }
    }

    // Busy: we can only update the client for now
    while(state_->isBusy() || !queueEmpty()) {
        if (!isFlushing_ && !queueEmpty()) {
            // Only start flushing if previous writes have been completed
            // and if there is actually something in the queue
            flush(yield);
        }
    }

//    // Push message to all queues
//    auto message = beast::buffers_to_string(buffer.data());
//    state_->push(message);

    ADAPTIV_DEBUG_CERR("<-run(id:" << this << ')');
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
