/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_MACROS_HPP
#define ADAPTIV_MACROS_HPP

#define ADAPTIV_NAMESPACE_BEGIN namespace adaptiv {
#define ADAPTIV_NAMESPACE_END }

#define ADAPTIV_CLOUD_NAMESPACE_BEGIN namespace cloud {
#define ADAPTIV_CLOUD_NAMESPACE_END }

#define ADAPTIV_SERVER_NAMESPACE_BEGIN namespace server {
#define ADAPTIV_SERVER_NAMESPACE_END }

#define ADAPTIV_CLIENT_NAMESPACE_BEGIN namespace client {
#define ADAPTIV_CLIENT_NAMESPACE_END }

#define ADAPTIV_UTILITY_NAMESPACE_BEGIN namespace utility {
#define ADAPTIV_UTILITY_NAMESPACE_END }

#define ADAPTIV_TRAITS_NAMESPACE_BEGIN namespace traits {
#define ADAPTIV_TRAITS_NAMESPACE_END }

#define ADAPTIV_MATH_NAMESPACE_BEGIN namespace math {
#define ADAPTIV_MATH_NAMESPACE_END }

/// A C-style string with the current adaptiv version
#define ADAPTIV_VERSION_STRING "adaptiv_0.1"

#if defined(NDEBUG) || defined(SILENT_DEBUG)
/// When in release, this macro does not generate code
#define ADAPTIV_DEBUG_CERR(...)
#else
///  When debugging, this macro streams to cerr
#define ADAPTIV_DEBUG_CERR(...) std::cerr << __VA_ARGS__ << '\n'
#endif

#endif //ADAPTIV_MACROS_HPP