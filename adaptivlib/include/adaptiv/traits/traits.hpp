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
#include <experimental/type_traits>
#include <string>

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_TRAITS_NAMESPACE_BEGIN

// [detection idiom] ----------------------------------------------------- begin

/// Alias for true_type if Op<Args...> is valid. Otherwise alias for false_type.
template<template<class...> class Op, class... Args>
using is_detected = std::experimental::is_detected<Op, Args...>;

/**
 * Alias for Op<Args...> if it is well formed. Otherwise nonesuch.
 * @note Can be used for dependent lookups
 */
template<template<class...> class Op, class... Args>
using detected_t = std::experimental::detected_t<Op, Args...>;

/**
* Alias for a struct with two member typedefs.
* @typedef \c type Alias for Op<Args...> if it denotes a valid type. Otherwise
*  alias for Default.
* \@ypedef \c value_t equivalent to \c is_detected
*/
template< class Default, template<class...> class Op, class... Args>
using detected_or = std::experimental::detected_or<Default, Op, Args...>;

/// Non instantiatable type to indicate detection failure
using nonesuch = std::experimental::nonesuch;

template<template<class...> class Op, class... Args>
constexpr bool is_detected_v = std::experimental::is_detected_v<Op, Args...>;

template<class Default, template<class...> class Op, class... Args >
using detected_or_t = std::experimental::detected_or_t<Default, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
using is_detected_exact =
std::experimental::is_detected_exact<Expected, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
constexpr bool is_detected_exact_v =
               std::experimental::is_detected_exact_v<Expected, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible =
std::experimental::is_detected_convertible<To, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
constexpr bool is_detected_convertible_v =
               std::experimental::is_detected_convertible_v<To, Op, Args...>;

// [detection idiom] ------------------------------------------------------- end

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
