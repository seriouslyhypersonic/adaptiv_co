/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SERVER_STATUS_HPP
#define ADAPTIV_SERVER_STATUS_HPP

#include <iostream>


#include <adaptiv/macros.hpp>
#include <adaptiv/serialization/external/cereal/archives/json.hpp>
#include <adaptiv/definitions.hpp>
#include <adaptiv/utility/output.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

namespace responses {

/// Indicates the server status, also used as a welcome message to clients
struct ServerStatus
{
    bool busy;
    std::string activeTarget;
    std::string error;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(busy),
            CEREAL_NVP(activeTarget),
            CEREAL_NVP(error));
    }

    friend std::ostream& operator<<(
        std::ostream& out,
        ServerStatus const& status)
    {
        namespace style = adaptiv::utility::output::style;

        auto column = std::setw(def::output::fieldw);

        out <<
            column << std::right << "busy: " << std::boolalpha <<
            column << std::left << status.busy;

        if (!status.activeTarget.empty()) {
            out << '\n' <<
                column << std::right << "routine: " <<
                style::message << column << std::left <<
                    status.activeTarget << style::none;
        }

        if (!status.error.empty()) {
            out << '\n' <<
                column << std::right << "error: " <<
                style::error << column <<
                    std::left << status.error << style::none;
        }
        return out;
    }
};

} // namespace response

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_SERVER_STATUS_HPP
