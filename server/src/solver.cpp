/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <string>

#include <adaptiv/cloud/protocol/response.hpp>
#include <adaptiv/cloud/protocol/messages/rans.hpp>

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

    residuals_.momentum.x = random(Residuals::min, Residuals::max);
    residuals_.momentum.y = random(Residuals::min, Residuals::max);
    residuals_.momentum.z = random(Residuals::min, Residuals::max);
    residuals_.energy     = random(Residuals::min, Residuals::max);
    residuals_.tke        = random(Residuals::min, Residuals::max);
    residuals_.tdr        = random(Residuals::min, Residuals::max);

    std::this_thread::sleep_for(iterationTime_); // Simulate runtime
}

std::string RANS::response()
{
    protocol::RANSResponse result {
        iteration_,
        {   // Residuals
            {   // Momentum
                residuals_.momentum.x,
                residuals_.momentum.y,
                residuals_.momentum.z
            },
            residuals_.energy,
            residuals_.tke,
            residuals_.tdr
        },
        // Busy
        iteration_ < maxIterations_,
        // Error
        ""
    };

    cloud::protocol::Response response("solve", result);
    return response.json();
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

        // Push payload to the queue of each active session
        state_->enqueue(response());
        ADAPTIV_DEBUG_CERR(response());
    }
//    state_->enqueue("{result:solverFinished}");
    state_->isBusy(false);
}

} // namespace solver

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
