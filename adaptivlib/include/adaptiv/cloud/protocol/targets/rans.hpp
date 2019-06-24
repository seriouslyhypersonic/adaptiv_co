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

#include <adaptiv/macros.hpp>
#include <adaptiv/serialization/external/cereal/details/traits.hpp>
#include <adaptiv/serialization/external/cereal/archives/json.hpp>

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

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
            CEREAL_NVP(momentum),
            CEREAL_NVP(energy),
            CEREAL_NVP(tke),
            CEREAL_NVP(tdr));
        }
    }           residuals;

    bool busy;

    std::string error;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
        CEREAL_NVP(iteration),
        CEREAL_NVP(residuals),
        CEREAL_NVP(busy),
        CEREAL_NVP(error));
    }
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_RANS_HPP
