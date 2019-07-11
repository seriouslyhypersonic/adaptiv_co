/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_NUMERIC_PARSERS_HPP
#define ADAPTIV_NUMERIC_PARSERS_HPP

#include <adaptiv/macros.hpp>

#include <boost/spirit/home/x3.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

/// Numeric X3 parsers
namespace parsers::numeric {

namespace x3 = boost::spirit::x3;

/**
 * @brief Metafunction that maps a specific numeric type to an X3 parser
 * @tparam T The numeric type to associate with the X3 parser
 * @typedef \c attribute_type type alias for the corresponding X3
 * numeric parser attribute (i.e. type \c T)
 * @typedef \c tag type alias for the respective x3::rule tag
 * @static Member \c name for the x3::rule name
 * @static Member \c parser_def, i.e. the underlying X3 parser
 */
template<class T>
struct get_numeric_parser { };

template<>
struct get_numeric_parser<float>
{
    using attribute_type = float;
    using            tag = class float_tag;

    inline static char const*    name   = "expecting float";
    inline static x3::float_type parser = x3::float_;
};

template<>
struct get_numeric_parser<double>
{
    using attribute_type = double;
    using            tag = class double_tag;

    inline static char const*     name   = "expecting double";
    inline static x3::double_type parser = x3::double_;
};

template<>
struct get_numeric_parser<short>
{
    using attribute_type = short;
    using            tag = class short_tag;

    inline static char const*    name   = "expecting short";
    inline static x3::short_type parser = x3::short_;
};

template<>
struct get_numeric_parser<unsigned short>
{
    using attribute_type = unsigned short;
    using            tag = class unsigned_short_tag;

    inline static char const*     name   = "expecting unsigned short";
    inline static x3::ushort_type parser = x3::ushort_;
};

template<>
struct get_numeric_parser<int>
{
    using attribute_type = int;
    using            tag = class int_tag;

    inline static char const*  name   = "expecting int";
    inline static x3::int_type parser = x3::int_;
};

template<>
struct get_numeric_parser<unsigned int>
{
    using attribute_type = unsigned int;
    using            tag = class uint_tag;

    inline static char const*  name   = "expecting unsigned int";
    inline static x3::uint_type parser = x3::uint_;
};

// No x3::uint_ parser

template<>
struct get_numeric_parser<long>
{
    using attribute_type = long;
    using            tag = class long_tag;

    inline static char const*   name   = "expecting long";
    inline static x3::long_type parser = x3::long_;
};

template<>
struct get_numeric_parser<unsigned long>
{
    using attribute_type = unsigned long;
    using            tag = class unsigned_long_tag;

    inline static char const*    name   = "expecting unsigned long";
    inline static x3::ulong_type parser = x3::ulong_;
};

template<>
struct get_numeric_parser<long long>
{
    using attribute_type = long long;
    using            tag = class long_long_tag;

    inline static char const*   name   = "expecting long long";
    inline static x3::long_type parser = x3::long_;
};

template<>
struct get_numeric_parser<unsigned long long>
{
    using attribute_type = unsigned long long;
    using            tag = class unsigned_long_long_tag;

    inline static char const*        name   = "expecting unsigned long long";
    inline static x3::ulong_longtype parser = x3::ulong_long;
};

} // namespace parsers::numeric

ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_NUMERIC_PARSERS_HPP
