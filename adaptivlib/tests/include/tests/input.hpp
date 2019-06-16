/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_TEST_INPUT_HPP
#define ADAPTIV_TEST_INPUT_HPP

#include <string>
#include <vector>
#include <type_traits>
#include <cstddef>
#include <algorithm>

#include <adaptiv/math/random.hpp>

/// Fill a vector with random numbers
template<class Arithmetic>
std::vector<Arithmetic> randomVector(
    std::size_t size,
    Arithmetic min = 0,
    Arithmetic max = 1)
{
    static_assert(std::is_arithmetic_v<Arithmetic>,
        "[randomVector] requires an arithmetic type");

    std::vector<Arithmetic> randomNumbers(size);
    std::generate(randomNumbers.begin(), randomNumbers.end(),
        [min, max](){
            return adaptiv::math::random<Arithmetic>(min, max);
        });
    return randomNumbers;
}

/// Generate a comma separated list of numbers
template<class T>
std::string makeList(
    std::vector<T> const& elems,
    std::string const& separator)
{
    static_assert(std::is_arithmetic_v<T>,
        "[makeList] requires an arithmetic type");

    std::string list;
    for (std::size_t i = 0; i < elems.size(); ++i) {
        list.append(std::to_string(elems[i]));
        if (i != elems.size() - 1) {
            list.append(separator);
        }
    }
    return list;
}

/// Generate a comma separated list of strings
template<>
std::string makeList(
    std::vector<std::string> const& elems,
    std::string const& separator)
{
    std::string list;
    for (std::size_t i = 0; i < elems.size(); ++i) {
        list.append("\"" + elems[i] + "\"");
        if (i != elems.size() - 1) {
            list.append(separator);
        }
    }
    return list;
}

/// Generate an array of numbers
template<class T>
std::string makeArray(std::vector<T> const& elems)
{
    static_assert(std::is_arithmetic_v<T>,
                  "[makeArray] requires an arithmetic type");

    auto array = makeList(elems, " ");
    return "[" + array + "]";
}

/// Generate an array of strings
template<>
std::string makeArray(std::vector<std::string> const& elems)
{
    auto array = makeList(elems, " ");
    return "[" + array + "]";
}

#endif //ADAPTIV_TEST_INPUT_HPP
