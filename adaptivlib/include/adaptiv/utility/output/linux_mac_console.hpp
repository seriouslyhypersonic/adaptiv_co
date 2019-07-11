/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_LINUX_MAC_CONSOLE_HPP
#define ADAPTIV_LINUX_MAC_CONSOLE_HPP

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_OUTPUT_NAMESPACE_BEGIN

namespace linux_mac {
char const* const csi = "\033["; // unix control sequence introducer
} // namespace linux_mac

ADAPTIV_OUTPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_LINUX_MAC_CONSOLE_HPP
