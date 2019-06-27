/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SYSTEM_HPP
#define ADAPTIV_SYSTEM_HPP

#include <iostream>
#include <stdexcept>
#include <string>

#include <adaptiv/macros.hpp>
#include <adaptiv/utility/output.hpp>

#include <boost/system/system_error.hpp>

ADAPTIV_NAMESPACE_BEGIN

using error_code = boost::system::error_code;

/// Report a failure on std::cerr
inline void fail(std::string const& message)
{
    namespace style = adaptiv::utility::output::style;
    std::cerr << style::error << message << style::none  <<"\n";
}

/**
 * Report a failure on std::cerr
 * @param ec the system error_code that generated the failure
 * @param what additional label to prepend to the error_code message
 */
inline void fail(error_code ec, char const* what)
{

    return fail(std::string(what) + ": " + ec.message());
}

/**
 * Throw a runtime_error
 * @param ec the system error_code that generated the failure
 * @param what additional label to prepend to the error_code message
 */
inline void except(error_code ec, char const* what) noexcept(false)
{
    throw std::runtime_error(std::string(what) + ": " + ec.message());
}

ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_SYSTEM_HPP
