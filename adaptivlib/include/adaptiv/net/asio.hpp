/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_ASIO_HPP
#define ADAPTIV_ASIO_HPP

#include <adaptiv/macros.hpp>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_NET_NAMESPACE_BEGIN

using namespace boost::asio; // From <boost/asio.hpp>
using tcp  = ip::tcp;        // From <boost/asio/web/tcp.hpp>

ADAPTIV_NET_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_ASIO_HPP
