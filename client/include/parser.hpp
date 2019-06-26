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

inline void doParser()
{
    namespace input = adaptiv::utility::input;

    std::cout <<
        "   Test parsing:\n"
        "       1) dArray [d1 d2 ...]\n"
        "       2) sArray [\"str1\" \"str2\" ...]\n"
        "       3) uInt uint1, unit2, ...\n";

    input::LineParser parser(std::cin);
    parser.getline("parser>");

    auto command = parser.popFront();

    bool success = false;
    if (command == "dArray") {
        std::vector<double> doubles;
        success = parser.parse(input::ParserType::Array, doubles);
        success? print(doubles) : parser.fail();
    } else if (command == "sArray") {
        std::vector<std::string> strings;
        success = parser.parse(input::ParserType::Array, strings);
        success? print(strings) : parser.fail();
    } else if (command == "uInt") {
        std::vector<unsigned long> ulongs;
        success = parser.parse(input::ParserType::List, ulongs);
        success? print(ulongs) : parser.fail();
    } else {
        std::cerr << "error: invalid command '" << command << "'\n";
    }

}

} // namespace client

#endif //PARSER_HPP
