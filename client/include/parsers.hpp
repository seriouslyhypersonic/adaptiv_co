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

inline void parsers()
{
    namespace input = adaptiv::utility::input;

    std::cout <<
        "   Test parsing:\n"
        "       1) dArray [d1 d2 ...]\n"
        "       2) sArray [\"str1\" \"str2\" ...]\n"
        "       3) uInt uint1, unit2, ...\n"
        "       4) x3 uint1, unit2, ...\n"
        "       5) x3a [\"str1\" \"str2\" ...]\n";

    input::LineParser parser(std::cin);
    parser.getline("parser>");

    auto command = parser.popFront();

    bool isParsed = false;
    if (command == "dArray") {
        std::vector<double> doubles;
        isParsed = parser.parse(input::ParserType::Array, doubles);
        isParsed? print(doubles) : parser.fail();
    } else if (command == "sArray") {
        std::vector<std::string> strings;
        isParsed = parser.parse(input::ParserType::Array, strings);
        isParsed? print(strings) : parser.fail();
    } else if (command == "uInt") {
        std::vector<unsigned long> ulongs;
        isParsed = parser.parse(input::ParserType::List, ulongs);
        isParsed ? print(ulongs) : parser.fail();
    } else if (command == "x3") {
        std::vector<unsigned int> uints;
        isParsed = parser.parse(input::ParserType::List, uints);
        isParsed ? print(uints) : parser.fail();
    } else if (command == "x3a") {
        std::vector<std::string> strings;
        isParsed = parser.parse(input::ParserType::Array, strings);
        isParsed ? print(strings) : parser.fail();
    } else {
        std::cerr << "error: invalid command '" << command << "'\n";
    }

}

} // namespace client

#endif //PARSER_HPP
