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

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_TRAITS_NAMESPACE_BEGIN

/**
 * Check if a type is a specialization of another type
 * @tparam Test The type to test
 * @tparam Ref The type to check if \c Test is a specialization of
 * @note This test failed
 */
template<class Test, template<class...> class Ref>
struct is_specialization : std::false_type {};

/**
 * Check if a type is a specialization of another type
 * @tparam Test The type to test
 * @tparam Ref The type to check if \c Test is a specialization of
 * @note This test succeded
 */
template<template<class...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type {};

template<class Test, template<class...> class Ref>
bool constexpr is_specialization_v = is_specialization<Test, Ref>::value;

ADAPTIV_TRAITS_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_TRAITS_HPP
