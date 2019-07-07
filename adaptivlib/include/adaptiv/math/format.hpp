/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_FORMAT_HPP
#define ADAPTIV_FORMAT_HPP

#include <string>
#include <sstream>
#include <iomanip>

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_MATH_NAMESPACE_BEGIN

template<class T>
std::string toScientific(T val, int precision = 6)
{
    static_assert(std::is_arithmetic_v<T>, "not a numeric type");
    std::ostringstream scientific;
    scientific << std::setprecision(precision) << std::scientific << val;
    return scientific.str();
}

ADAPTIV_MATH_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_FORMAT_HPP
