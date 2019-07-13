/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <sys/ioctl.h> // ioctl, TIOCGWINSZ
#include <unistd.h>    // STDOUT_FILENO

#include <adaptiv/utility/output/posix_console.hpp>

std::size_t adaptiv::utility::output::posix::consoleWidth()
{
    static std::size_t const failure = 0;
    winsize window { };

    // TIOCGWINSZ tells to fill the winsize struct corresponding
    // to the standard output file descriptor STDOUT_FILENO
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) < 0 ) {
        return failure;
    }

    return window.ws_col;
}