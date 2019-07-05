/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_PARSERS_HPP
#define ADAPTIV_PARSERS_HPP

#include <adaptiv/macros.hpp>
#include <adaptiv/traits/traits.hpp>
#include <adaptiv/utility/input/auxiliary_parsers.hpp>
#include <adaptiv/utility/input/character_parsers.hpp>
#include <adaptiv/utility/input/numeric_parsers.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

/// Static assertion: assert type \c T is std::vector                           todo: move to macros.hpp
#define ADAPTIV_ASSERT_STD_VECTOR(T, ...)       \
static_assert(                                  \
    traits::is_specialization_v<T, std::vector>,\
    __VA_ARGS__)                                \

/// Static assertion: assert type \c T is arithmetic or std::string             todo: move to macros.hpp
#define ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T, ...)         \
static_assert(                                              \
    std::is_arithmetic_v<T> || traits::is_basic_string_v<T>,\
    __VA_ARGS__)

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

/// X3 parsers conforming to adaptiv-specific grammars
namespace parsers {

namespace x3 = boost::spirit::x3;

/// The error handler used to report a parsing failure
struct ErrorHandler
{
    template<class Iterator, class Exception, class Context>
    x3::error_handler_result on_error(
    Iterator& first,
    Iterator const& last,
    Exception const& exception,
    Context const& context)
    {
        // Get the error handler from the parser context
        auto& error_handler = x3::get<x3::error_handler_tag>(context).get();

        // Pass the position and error message
        std::string message =
                    "parse error: " + exception.which() + " here:";
        error_handler(exception.where(), message);

        // The parser will quit and fail immediately
        return x3::error_handler_result::fail;
    }
};

namespace detail {
// Metafunction to create a tag that inherits the ErrorHandler when required
template<bool withErrorHandling>
struct make_tag { };

template<>
struct make_tag<true>: ErrorHandler { };
} // namespace detail

/**
 * Make an X3 parser using a grammar for comma separated elements
 * @tparam T The type of list elements
 * @tparam withErrorHandling Flag used to provide error handling support
 * @return The X3 parser according to the type of list elements
 * @note Numeric types are parsed according to their specific X3 numeric parser
 * @note Strings are parsed as quoted text
 */
template<class T, bool withErrorHandling = true>
auto makeListParser()
{
    ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T,
        "list elements must be an arithmetic type or a std::string");

    struct list_tag: detail::make_tag<withErrorHandling> { };

    using auxiliary::eolist;
    using character::comma;

    if constexpr (std::is_arithmetic_v<T>) {
        // The numeric parser for type T
        using num_tag  = typename numeric::get_numeric_parser<T>::tag;
        auto num_name  = numeric::get_numeric_parser<T>::name;
        auto num_def   = numeric::get_numeric_parser<T>::parser;

        auto const num = x3::rule<num_tag, T>{num_name}
                       = num_def;

        // The parser for a numeric list
        auto const list
            = x3::rule<list_tag, std::vector<T>>{"numeric list"}
            = x3::expect[num] > (eolist | (*(comma > num) > eolist));

        return list;
    } else {
        // The quoted string parser
        using character::quotedString;

        // The parser for a list of quoted strings
        auto const list
            = x3::rule<list_tag, std::vector<T>>{"quoted string list"}
            = x3::expect[quotedString] >
                (eolist | (*(comma > quotedString) > eolist));

        return list;
    }
}

/**
 * Make an X3 parser using a grammar for arrays: [elem1 elem2 ... elemN]
 * @tparam T The type of array elements
 * @tparam withErrorHandling Flag used to provide error handling support
 * @return The X3 parser according to the type of array elements
 * @note Numeric types are parsed according to their specific X3 numeric parser
 * @note Strings are parsed as quoted text
 */
template<class T, bool withErrorHandling = true>
auto makeArrayParser()
{
    ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T,
        "array elements must be an arithmetic type or a std::string");

    struct array_tag: detail::make_tag<withErrorHandling> { };

    using character::obracket;
    using character::cbracket;

    if constexpr (std::is_arithmetic_v<T>) {
        // The numeric parser for type T
        using num_tag  = typename numeric::get_numeric_parser<T>::tag;
        auto num_name  = numeric::get_numeric_parser<T>::name;
        auto num_def   = numeric::get_numeric_parser<T>::parser;

        auto const num = x3::rule<num_tag, T>{num_name}
                       = num_def;

        // The parser for a numeric array
        auto const array = x3::rule<array_tag, std::vector<T>>{"numeric array"}
                         = x3::expect[obracket] > *num > cbracket;

        return array;
    } else {
        // The quoted string parser
        using character::quotedString;

        // The parser for an array of quoted strings
        auto const array
            = x3::rule<array_tag, std::vector<T>>{"quoted string array"}
            = x3::expect[obracket] > *quotedString > cbracket;

        return array;
    }
}

} // namespace parsers

ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_PARSERS_HPP
