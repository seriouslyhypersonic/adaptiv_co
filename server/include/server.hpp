/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SERVER_HPP
#define ADAPTIV_SERVER_HPP

#include <string>
#include <memory>
#include <unordered_set>

#include <adaptiv/net/net.hpp>
#include <adaptiv/cloud/cloud.hpp>

#include "shared_state.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

/// Accepts incoming connections and launches the sessions
void doListen(
    net::io_context& context,
    net::tcp::endpoint endpoint,
    std::shared_ptr<SharedState> const& state,
    net::yield_context yield);

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_SERVER_HPP
