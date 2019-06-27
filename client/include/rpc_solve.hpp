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

/// Call target solve on the server
void rpcSolve(
    std::string const& host,
    std::string const& port);

void doSolve(std::string const& host, std::string const& port);

} // namespace client

#endif //ADAPTIV_CORO_RPC_SOLVE_H
