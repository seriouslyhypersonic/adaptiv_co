/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

#include <adaptiv/utility/input.hpp>

namespace client {

template<class T>
void print(T const& ts)
{
    for (auto const t : ts) {
        std::cout << '{' << t << "} ";
    }
    std::cout << '\n';
}

void parsers();

} // namespace client

#endif //PARSER_HPP
