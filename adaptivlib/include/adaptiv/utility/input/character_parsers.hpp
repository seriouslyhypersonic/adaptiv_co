/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_CHARACTER_PARSERS_HPP
#define ADAPTIV_CHARACTER_PARSERS_HPP

#include <adaptiv/macros.hpp>

#include <boost/spirit/home/x3.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

/// Character (and string) X3 parsers
namespace parsers::character {

namespace x3 = boost::spirit::x3;

/// Parse a comma literal ','
auto const comma = x3::rule<class comma_tag> {"missing ','"}
                 = x3::lit(',');

/// Parse a quotation mark
auto const quotes = x3::rule<class quotes_tg> {"missing \""}
                  = x3::lit('"');

/// Parse an opening bracket
auto const obracket = x3::rule<class obracket_tag> {"missing ["}
                    = x3::lit('[');

/// Parse a closing bracket
auto const cbracket = x3::rule<class cbracket_tag> {"missing ]"}
                    = x3::lit(']');

/// Parse a quoted string
auto const quotedString
    = x3::rule<class quotedString_tag, std::string> {"expecting quoted string"}
    = x3::lexeme[quotes > +(x3::char_ - quotes) > quotes];
} // namespace parsers::character

ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_CHARACTER_PARSERS_HPP
