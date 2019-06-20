/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "solver.hpp"

#include "shared_state.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

namespace solver {

void RANS::update()
{
    using adaptiv::math::random;

    ++iteration_;

    if (iteration_ == 1) return;

    residuals_.momentum_.x_ = random(Residuals::min, Residuals::max);
    residuals_.momentum_.y_ = random(Residuals::min, Residuals::max);
    residuals_.momentum_.z_ = random(Residuals::min, Residuals::max);
    residuals_.energy_      = random(Residuals::min, Residuals::max);
    residuals_.tke_         = random(Residuals::min, Residuals::max);
    residuals_.tdr_         = random(Residuals::min, Residuals::max);

    std::this_thread::sleep_for(iterationTime_); // Simulate runtime
}

RANS::RANS(
    SharedState* state,
    size_t maxIterations,
    miliseconds_t iterationTime)
    : state_(state)
    , maxIterations_(maxIterations)
    , iterationTime_(iterationTime)
    , residuals_{{1, 1, 1}, 1, 1, 1}
{ }

void RANS::run()
{
    while (iteration_ < maxIterations_) {
        update();

        // Prepare iteration payload
        std::ostringstream msg;
        msg <<
            "{iteration: " << std::setw(5) << iteration_ <<
            ", residuals: " << residuals_ <<
            "}";

        // Push payload to the queue of each active session
        state_->enqueue(msg.str());
        ADAPTIV_DEBUG_CERR(msg.str());
    }
    state_->enqueue("{result:solverFinished}");
    state_->isBusy(false);
}

} // namespace solver

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
