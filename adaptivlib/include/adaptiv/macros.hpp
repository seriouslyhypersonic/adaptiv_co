/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_MACROS_HPP
#define ADAPTIV_MACROS_HPP

// [platform] ------------------------------------------------------------------
#ifdef __linux__
    #define ADAPTIV_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
    #define ADAPTIV_MAC
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(_WIN64)
    #define ADAPTIV_WINDOWS
#endif

// [namespaces] ----------------------------------------------------------------

#define ADAPTIV_NAMESPACE_BEGIN namespace adaptiv {
#define ADAPTIV_NAMESPACE_END }

#define ADAPTIV_CLOUD_NAMESPACE_BEGIN namespace cloud {
#define ADAPTIV_CLOUD_NAMESPACE_END }

#define ADAPTIV_CLIENT_NAMESPACE_BEGIN namespace client {
#define ADAPTIV_CLIENT_NAMESPACE_END }

#define ADAPTIV_PROTOCOL_NAMESPACE_BEGIN namespace protocol {
#define ADAPTIV_PROTOCOL_NAMESPACE_END }

#define ADAPTIV_SERVER_NAMESPACE_BEGIN namespace server {
#define ADAPTIV_SERVER_NAMESPACE_END }

#define ADAPTIV_MATH_NAMESPACE_BEGIN namespace math {
#define ADAPTIV_MATH_NAMESPACE_END }

#define ADAPTIV_NET_NAMESPACE_BEGIN namespace net {
#define ADAPTIV_NET_NAMESPACE_END }

#define ADAPTIV_SERIALIZATION_NAMESPACE_BEGIN namespace serialization {
#define ADAPTIV_SERIALIZATION_NAMESPACE_END }

#define ADAPTIV_UTILITY_NAMESPACE_BEGIN namespace utility {
#define ADAPTIV_UTILITY_NAMESPACE_END }

#define ADAPTIV_INPUT_NAMESPACE_BEGIN namespace input {
#define ADAPTIV_INPUT_NAMESPACE_END }

#define ADAPTIV_OUTPUT_NAMESPACE_BEGIN namespace output {
#define ADAPTIV_OUTPUT_NAMESPACE_END }

#define ADAPTIV_TRAITS_NAMESPACE_BEGIN namespace traits {
#define ADAPTIV_TRAITS_NAMESPACE_END }

// [version] -------------------------------------------------------------------

/// A C-style string with the current adaptiv version
#define ADAPTIV_VERSION_STRING "adaptiv_0.1"

// [debugging] -----------------------------------------------------------------

#if defined(NDEBUG) || defined(SILENT_DEBUG)
/// When in release, this macro does not generate code
#define ADAPTIV_DEBUG_CERR(...)
#else
///  When debugging, this macro streams to cerr
#define ADAPTIV_DEBUG_CERR(...) std::cerr << __VA_ARGS__ << '\n'
#endif

#endif //ADAPTIV_MACROS_HPP