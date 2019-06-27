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

namespace protocol = adaptiv::cloud::protocol;

/// Ping the server
protocol::responses::ServerStatus rpcPing(
    std::string const& host,
    std::string const& port) noexcept(false);

void doPing(std::string const& host, std::string const& port);

} // namespace client

#endif //ADAPTIV_RPC_PING_HPP
