/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_DATETIME_HPP
#define ADAPTIV_DATETIME_HPP

#include <string>

#include <adaptiv/macros.hpp>

#include <adaptiv/serialization/external/cereal/archives/json.hpp>
#include <adaptiv/serialization/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

namespace response {
/// A date-time response
struct DateTime
{
    std::string date;
    std::string time;
    std::string error;

    ADAPTIV_SERIALIZE(date, time, error);
};
} // namespace request

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_DATETIME_HPP
