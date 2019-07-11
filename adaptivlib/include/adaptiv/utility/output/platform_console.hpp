/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_PLATFORM_CONSOLE_HPP
#define ADAPTIV_PLATFORM_CONSOLE_HPP

#include <adaptiv/macros.hpp>

#ifdef ADAPTIV_WINDOWS
#include <adaptiv/utility/output/windows_console.hpp>
#else
#include <adaptiv/utility/output/linux_mac_console.hpp>
#endif

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_OUTPUT_NAMESPACE_BEGIN

namespace platform {
#ifdef ADAPTIV_WINDOWS
char const* const csi = windows::csi; // control sequence introducer
bool const hasOutputSequenceSupport = windows::enableOutputSequences();
#else
char const* const csi = linux_mac::csi; // control sequence introducer
const bool hasOutputSequenceSupport = true; // todo: actually check for support
#endif
} // namespace platform

ADAPTIV_OUTPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_PLATFORM_CONSOLE_HPP
