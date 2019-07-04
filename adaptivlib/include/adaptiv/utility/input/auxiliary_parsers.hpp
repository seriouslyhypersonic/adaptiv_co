/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_AUXILIARY_PARSERS_HPP
#define ADAPTIV_AUXILIARY_PARSERS_HPP

#include <adaptiv/macros.hpp>

#include <boost/spirit/home/x3.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

namespace x3_parsers {

namespace x3 = boost::spirit::x3;

/// End of list parser (fails because of a missing ',')
auto const eolist = x3::rule<class eolist_tag> {"missing ','"}
                  = x3::eoi;

} // namespace x3_parsers

ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_AUXILIARY_PARSERS_HPP
