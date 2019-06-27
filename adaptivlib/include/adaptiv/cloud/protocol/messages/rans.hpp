/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_RANS_HPP
#define ADAPTIV_RANS_HPP

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <string>

#include <adaptiv/macros.hpp>
#include <adaptiv/serialization/external/cereal/details/traits.hpp>
#include <adaptiv/serialization/external/cereal/archives/json.hpp>
#include <adaptiv/math/format.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

/// A RANS response
struct RANSResponse
{
    std::size_t iteration;

    struct Residuals
    {
        struct Momentum
        {
            double x, y, z;

            /// Make momentum residuals serializable
            template<class Archive>
            void serialize(Archive& archive)
            {
                archive(
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(z));
            }

        } momentum;

        double energy;
        double tke;
        double tdr;

        /// Make residuals serializable
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
            CEREAL_NVP(momentum),
            CEREAL_NVP(energy),
            CEREAL_NVP(tke),
            CEREAL_NVP(tdr));
        }
    } residuals;

    bool busy;

    std::string error;

    /// Make the response serializable
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
        CEREAL_NVP(iteration),
        CEREAL_NVP(residuals),
        CEREAL_NVP(busy),
        CEREAL_NVP(error));
    }

    static std::string& header()
    {
        static std::string msg;
        if (msg.empty()) {
            std::ostringstream out;
            out << '\n' << std::setw(15) <<
                "iteration | " << std::setw(15) <<
                "mom X     | " << std::setw(15) <<
                "mom Y     | " << std::setw(15) <<
                "mom Z     | " << std::setw(15) <<
                "energy    | " << std::setw(15) <<
                "Tke       | " << std::setw(15) <<
                "Tdr       | " << '\n';
            msg = out.str();
        }
        return msg;
    }

    friend std::ostream& operator<<(
        std::ostream& out,
        RANSResponse const& response)
    {
        using adaptiv::math::toScientific;

        return out << std::setw(15) << std::right <<
            std::to_string(response.iteration) + " | " <<
            toScientific(response.residuals.momentum.x) + " | " <<
            toScientific(response.residuals.momentum.y) + " | " <<
            toScientific(response.residuals.momentum.z) + " | " <<
            toScientific(response.residuals.energy) + " | " <<
            toScientific(response.residuals.tke) + " | " <<
            toScientific(response.residuals.tdr) + " | " << '\n';
    }
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_RANS_HPP
