/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "parsers.hpp"

void client::parsers()
{
    namespace input = adaptiv::utility::input;
    using input::ParserType;

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
        isParsed = parser.parse(ParserType::Array, doubles);
        isParsed? print(doubles) : parser.fail();
    } else if (command == "sArray") {
        std::vector<std::string> strings;
        isParsed = parser.parse(ParserType::Array, strings);
        isParsed? print(strings) : parser.fail();
    } else if (command == "uInt") {
        std::vector<unsigned long> ulongs;
        isParsed = parser.parse(ParserType::List, ulongs);
        isParsed ? print(ulongs) : parser.fail();
    } else if (command == "x3") {
        std::vector<unsigned int> uints;
        isParsed = parser.parse(ParserType::List, uints);
        isParsed ? print(uints) : parser.fail();
    } else if (command == "x3a") {
        std::vector<std::string> strings;
        isParsed = parser.parse(ParserType::Array, strings);
        isParsed ? print(strings) : parser.fail();
    } else {
        std::cerr << "error: invalid command '" << command << "'\n";
    }

}