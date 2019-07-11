/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_TRAITS_HPP
#define ADAPTIV_TRAITS_HPP

#include <type_traits>
#include <string>

#include <adaptiv/macros.hpp>
#include <adaptiv/traits/detection_idiom.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_TRAITS_NAMESPACE_BEGIN

// traits::is_specialization ---------------------------------------------------

/**
 * Check if a type is a specialization of another type
 * @tparam Test The type to test
 * @tparam Ref The type to check if \c Test is a specialization of
 * @note This test failed
 */
template<class Test, template<class...> class Ref>
struct is_specialization : std::false_type { };

/**
 * Check if a type is a specialization of another type
 * @tparam Test The type to test
 * @tparam Ref The type to check if \c Test is a specialization of
 * @note This test succeeded
 */
template<template<class...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type { };

template<class Test, template<class...> class Ref>
bool constexpr is_specialization_v = is_specialization<Test, Ref>::value;

// traits::is_basic_string -----------------------------------------------------

/**
 * Check if T is a basic_string type
 * @tparam T
 * @note This test failed
 */
template<class T>
struct is_basic_string: std::false_type { };

/**
 * Check if T is a basic_string type
 * @tparam T
 * @note This test succeeded
 */
template<class ChartT, class Traits, class Allocator>
struct is_basic_string<std::basic_string<ChartT, Traits, Allocator>>:
    std::true_type { };

template<class T>
bool constexpr is_basic_string_v = is_basic_string<T>::value;

ADAPTIV_TRAITS_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_TRAITS_HPP
