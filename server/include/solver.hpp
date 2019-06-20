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
            double x_, y_, z_;
        } momentum_;

        double energy_;
        double tke_;
        double tdr_;

        friend
        std::ostream& operator<<(std::ostream& out, Residuals const& residuals)
        {
            return out <<
                       "{momX: "    << std::setw(10) << residuals.momentum_.x_ <<
                       ", momY: "   << std::setw(10) << residuals.momentum_.y_ <<
                       ", momZ: "   << std::setw(10) << residuals.momentum_.z_ <<
                       ", energy: " << std::setw(10) << residuals.energy_ <<
                       ", tke: "    << std::setw(10) << residuals.tke_ <<
                       ", tdr: "    << std::setw(10) << residuals.tdr_ <<
                       "}";
        }
    } residuals_;

    void update();

public:
    explicit RANS(
        SharedState* state,
        std::size_t maxIterations = 32,
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
