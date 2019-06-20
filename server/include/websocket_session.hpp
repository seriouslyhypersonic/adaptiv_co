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
    friend class SharedState; ///< SharedState needs access

    /// A queue of immutable shared strings
    using queue_t = std::queue<std::shared_ptr<std::string const>>;

    /// The underlying I/O object for current session
    beast::websocket::stream<beast::tcp_stream> websocket_;

    /// The server state, accessible from all active sessions
    std::shared_ptr<SharedState> state_;

    bool hasConnection_ = true; ///< Check that the connection is active

    /// To avoid initiating multiple invocations of flush
    bool isFlushing_ = false;

    // [shared resources] ------------------------------------------------ begin
    /// A per-session queue of outgoing messages
    queue_t queue_;

    // Thread-safe functions for the shared resource
    bool                 queueEmpty(); ///< Check empty \note Thread-safe
    queue_t::value_type& queueFront(); ///< First element \note Thread-safe
    void                 queuePop();   ///< Pop first element \note Thread-safe
    void                 queueClear(); ///< Clear the queue \note Thread-safe
    queue_t              queueCopy();  ///< Return a copy \note Thread-safe

    // [shared resources]  ------------------------------------------------- end

    /// Send a single message
    void message(std::string const& msg, boost::asio::yield_context yield);

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

    /// Send a welcome message
    void welcome(net::yield_context yield);

    /**
     * todo: probably delete this function and use flush (only)
     * Write the queue to the websocket
     * @attention This function is \b NOT thread-safe and should only be used if
     * the solver is not running (i.e. no risk in accessing the session queue)
     */
    void write(net::yield_context yield); // non thread safe

    /**
     * Flush the shared queue by copying its contents into a temporary queue
     * that is then written to the websocket. This function immediately clears
     * the shared queue making it available for other threads.
     *
     */
    void flush(net::yield_context yield); // Thread-safe

    /// Run the WebSocketSession
    void run(net::yield_context yield);

    /// Convenience function - make a shared_ptr
    static std::shared_ptr<WebSocketSession> makeShared(
        net::tcp::socket&& socket,
        std::shared_ptr<SharedState> const& state);

    bool hasConnection() const noexcept { return hasConnection_; }
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
