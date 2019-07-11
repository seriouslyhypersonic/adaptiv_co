/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_DETECTION_IDIOM_HPP
#define ADAPTIV_DETECTION_IDIOM_HPP

#ifdef _MSC_VER
//    #if _MSC_VER <= 1000
        #define NO_DETECTION_IDIOM
//    #endif
#else
    #include <experimental/type_traits>
    #define HAS_EXPERIMENTAL_DETECTION_IDIOM
#endif

#include <type_traits>

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_TRAITS_NAMESPACE_BEGIN

#ifdef NO_DETECTION_IDIOM

// [detection idiom] ----------------------------------------------------- begin

/// Used by detected_t to  indicate detection failure
struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

namespace detail {
template <class Default, class AlwaysVoid,
template<class...> class Op, class... Args>
struct detector {
    using value_t = std::false_type;
    using type = Default;
};

template <class Default, template<class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
    // Note that std::void_t is a C++17 feature
    using value_t = std::true_type;
    using type = Op<Args...>;
};

} // namespace detail

/// Alias for true_type if Op<Args...> is valid. Otherwise alias for false_type.
template <template<class...> class Op, class... Args>
using is_detected =
    typename detail::detector<nonesuch, void, Op, Args...>::value_t;

/**
 * Alias for Op<Args...> if it is well formed. Otherwise nonesuch.
 * @note Can be used for dependent lookups
 */
template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

/**
* Alias for a struct with two member typedefs.
* @typedef \c type Alias for Op<Args...> if it denotes a valid type. Otherwise
*  alias for Default.
* \@ypedef \c value_t equivalent to \c is_detected
*/
template <class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template< template<class...> class Op, class... Args >
constexpr bool is_detected_v = is_detected<Op, Args...>::value;

template< class Default, template<class...> class Op, class... Args >
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template <class Expected, template<class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

template <class Expected, template<class...> class Op, class... Args>
constexpr bool is_detected_exact_v =
    is_detected_exact<Expected, Op, Args...>::value;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible =
    std::is_convertible<detected_t<Op, Args...>, To>;

template <class To, template<class...> class Op, class... Args>
constexpr bool is_detected_convertible_v =
    is_detected_convertible<To, Op, Args...>::value;

// [detection idiom] ----------------------------------------------------- begin

#elif defined(HAS_EXPERIMENTAL_DETECTION_IDIOM)

// [experimental detection idiom] ---------------------------------------- begin

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

// [experimental detection idiom] ------------------------------------------ end

#endif

ADAPTIV_TRAITS_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_DETECTION_IDIOM_HPP
