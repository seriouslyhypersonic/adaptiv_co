/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_RANDOM_HPP
#define ADAPTIV_RANDOM_HPP

#include <type_traits>
#include <random>

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_MATH_NAMESPACE_BEGIN

template<class Arithmetic>
Arithmetic random(Arithmetic min = 0, Arithmetic max = 1)
{
    static_assert(std::is_arithmetic_v<Arithmetic>,
            "[adaptiv::math::random] requires an arithmetic type");

    static std::random_device randomDevice;
    static std::mt19937 rng(randomDevice());

    if constexpr (std::is_integral_v<Arithmetic>) {
        static std::uniform_int_distribution<Arithmetic> uniform(min, max);
        return uniform(rng);
    } else {
        std::uniform_real_distribution<Arithmetic> uniform(min, max);
        return uniform(rng);
    }
}

ADAPTIV_MATH_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_RANDOM_HPP
