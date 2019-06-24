/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SOLVER_HPP
#define ADAPTIV_SOLVER_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstddef>
#include <thread>
#include <chrono>

#include <adaptiv/macros.hpp>
#include <adaptiv/math/random.hpp>
#include <adaptiv/cloud/protocol/response.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

// Forward declare
class SharedState;

namespace solver {

using miliseconds_t = std::chrono::milliseconds;
using namespace std::chrono_literals;

/// A mock solver
class RANS
{
private:
    SharedState* state_;

    std::size_t maxIterations_;
    miliseconds_t iterationTime_;
    std::size_t iteration_ = 0;

    struct Residuals
    {
        static double constexpr min = 1e-8;
        static double constexpr max = 1;

        struct Momentum
        {
            double x, y, z;
        } momentum;

        double energy;
        double tke;
        double tdr;

        friend
        std::ostream& operator<<(std::ostream& out, Residuals const& residuals)
        {
            return out <<
                       "{momX: "    << std::setw(10) << residuals.momentum.x <<
                       ", momY: "   << std::setw(10) << residuals.momentum.y <<
                       ", momZ: "   << std::setw(10) << residuals.momentum.z <<
                       ", energy: " << std::setw(10) << residuals.energy <<
                       ", tke: "    << std::setw(10) << residuals.tke <<
                       ", tdr: "    << std::setw(10) << residuals.tdr <<
                       "}";
        }
    } residuals_;

    void update();
    std::string response();

public:
    explicit RANS(
        SharedState* state,
        std::size_t maxIterations = 500,
        miliseconds_t iterationTime = 500ms);

    /**
     * Run the solver until the maximum number of iterations.
     * @note Accesses the SharedState isBusy_ and enqueue() resources
     */
    void run();
};

} // namespace solver

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_SOLVER_HPP
