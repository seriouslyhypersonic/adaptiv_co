/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_WINDOWS_CONSOLE_HPP
#define ADAPTIV_WINDOWS_CONSOLE_HPP

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_OUTPUT_NAMESPACE_BEGIN

namespace windows {

char const* const csi = "\x1b["; // windows control sequence introducer

/**
 * Enables ANSI escape sequences in conhost.exe and cmd.exe under
 * Windows 10 TH2 (v1511)
 */
bool enableOutputSequences();

/**
 * Windows console width
 * @return The console width in characters or 0 if unable to determine the width
 */
std::size_t consoleWidth();
} // namespace windows

ADAPTIV_OUTPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_WINDOWS_CONSOLE_HPP
