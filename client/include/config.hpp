/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <ctime>

#include <adaptiv/utility/output.hpp>

namespace client {

unsigned short constexpr width = 50;

/// Get current date and time
inline std::string dateTime()
{
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    std::ostringstream out;
    out.imbue(std::locale(""));
    out << std::put_time(&tm, "%a %x %X");
    return out.str();
}

/// Display welcome message
inline void welcomeMessage()
{
    namespace output = adaptiv::utility::output;

    std::cout << std::string(width, '-') << '\n';
    std::cout << "    Welcome to " <<  output::style::message << "adaptiv" <<
        output::style::none << '\n';


    std::cout << std::setfill(' ');
    std::cout << std::right << std::setw(width) << dateTime() << '\n';
    std::cout << std::string(width, '-') << '\n';
}

/// Display help menu
inline void help()
{
    std::cout <<
        "List of commands\n" <<
        "     ping  - ping adaptiv server\n"
        "     solve - start solution process\n"
        "   details - connection details\n"
        "     help  - show this menu\n"
        "     quit  - close client\n";
}

} // namespace client

#endif //CONFIG_HPP
