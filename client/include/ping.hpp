/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_RPC_PING_HPP
#define ADAPTIV_RPC_PING_HPP

#include <adaptiv/cloud/protocol/messages/server_status.hpp>

namespace client {

namespace beast = adaptiv::beast;
namespace protocol = adaptiv::cloud::protocol;

/// Remote procedure calls
namespace rpc {
/**
 * Ping the server using an active WebSocket connection
 * @param websocket The TCP stream socket connected using WebSocket protocol
 * @return The server status response
 * @note A prior successful WebSocket handshake is required
 */
client::protocol::responses::ServerStatus
ping(beast::websocket::stream<beast::tcp_stream>& websocket);

/**
 * Ping the server using a new WebSocket connection
 * @return The server status response
 */
protocol::responses::ServerStatus
ping(std::string const& host, std::string const& port);
} // namespace rpc

/// Do the client ping
void ping(std::string const& host, std::string const& port);

} // namespace client

#endif //ADAPTIV_RPC_PING_HPP
