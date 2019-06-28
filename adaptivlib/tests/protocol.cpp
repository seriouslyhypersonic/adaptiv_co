/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <vector>
#include <sstream>
#include <cstddef>

#include <adaptiv/cloud/protocol/protocol.hpp>
#include <adaptiv/serialization/external/cereal/types/vector.hpp>
#include <adaptiv/serialization/external/cereal/types/memory.hpp>

#include <adaptiv/cloud/protocol/messages/date_time.hpp>

#include <tests/testing.hpp>

namespace protocol = adaptiv::cloud::protocol;

struct NotJsonSerializable { };
struct JsonSerializable
{
    template<class Archive>
    void serialize(Archive& archive)
    {

    }
};

struct SolveParams
{
    std::string solver;
    std::size_t maxIterations;
    std::vector<std::string> numbers;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(solver),
            CEREAL_NVP(maxIterations),
            CEREAL_NVP(numbers)
        );
    }
};

struct NoResponseError { };
struct SolveResult
{
    std::size_t iteration;

    struct Residuals
    {
        double momX, momY, momZ;

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(CEREAL_NVP(momX), CEREAL_NVP(momY), CEREAL_NVP(momZ));
        }
    } residuals;

    std::string error;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(iteration),
            CEREAL_NVP(residuals),
            CEREAL_NVP(error));
    }
};

TEST(Protocol, Request)
{
    std::string const solver("RANS");
    std::size_t const maxIterations = 32;
    std::vector<std::string> numbers = {"yi", "er", "san"};

    // Test traits
    ASSERT_FALSE(protocol::traits::is_json_serializable_v<NotJsonSerializable>);
    ASSERT_TRUE(protocol::traits::is_json_serializable_v<JsonSerializable>);

    // Test output serialization
    SolveParams params{solver, maxIterations, numbers};
    protocol::Request outReq("solve", params);
    
    // Simulate sending...
    auto in = outReq.json();

    // When receiving, first get the target, so we can provide the right
    // Message type to reconstruct the Request
    ASSERT_EQ(protocol::target(in), "solve");

    protocol::Request<SolveParams> inReq(outReq.json());

    // Check the reconstruction was successful
    ASSERT_EQ(outReq.message().solver, inReq.message().solver);
    ASSERT_EQ(outReq.message().maxIterations, inReq.message().maxIterations);
    ADAPTIV_ASSERT_ELEMENTS_EQ(
        outReq.message().numbers,
        inReq.message().numbers);
}

TEST(Protocol, Response)
{
    std::size_t const iterations = 12;
    double const momX = 0.32, momY = 3.14, momZ = 1.41;
    SolveResult result {iterations, {momX, momY, momZ}, ""};
    protocol::Response outResp("solve", result);

    // Test traits
    ASSERT_FALSE(protocol::traits::has_response_error_v<NoResponseError>);
    ASSERT_TRUE(protocol::traits::has_response_error_v<SolveResult>);

    // Simulate sending...
    auto in = outResp.json();
    
    // When receiving, first get the target, so we can provide the right
    // Message type to reconstruct the Response
    ASSERT_EQ(protocol::target(in), "solve");

    protocol::Response<SolveResult> inResp(in);

    // Check the reconstruction was successful
    ASSERT_EQ(outResp.message().iteration, inResp.message().iteration);
    ASSERT_EQ(outResp.message().residuals.momX,
        inResp.message().residuals.momX);
    ASSERT_EQ(outResp.message().residuals.momY,
        inResp.message().residuals.momY);
    ASSERT_EQ(outResp.message().residuals.momZ,
        inResp.message().residuals.momZ);
    ASSERT_EQ(outResp.message().error, inResp.message().error);
}
