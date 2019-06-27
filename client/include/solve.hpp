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
#include <sstream>
#include <cstddef>

#include <adaptiv/net/net.hpp>
#include <adaptiv/cloud/protocol/response.hpp>
#include <adaptiv/cloud/protocol/messages/rans.hpp>

namespace client {

namespace net = adaptiv::net;
namespace protocol = adaptiv::cloud::protocol;

/// Remote procedure calls
namespace rpc {
/// Call target solve on the server
void solve(std::string const& host, std::string const& port);
} // namespace rpc

/// Calls target solve on the server and monitors the solution process
void solve(std::string const& host, std::string const& port);

} // namespace client

#endif //ADAPTIV_CORO_RPC_SOLVE_H
