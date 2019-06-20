/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "shared_state.hpp"
#include "websocket_session.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

SharedState::SharedState(net::io_context& context,  std::string documentRoot)
    : context_(context)
    , documentRoot_(std::move(documentRoot))
    , ransSolver_(this)
{ }

SharedState::~SharedState()
{
    // The solver might still be running, so join here
    if (solverThread_.joinable()) {
        solverThread_.join();
    }
}

void SharedState::join(WebSocketSession* session)
{
    std::lock_guard guard(mutex_);
    sessions_.insert(session);
    ADAPTIV_DEBUG_CERR("join(id" << session << ')');
}

void SharedState::leave(WebSocketSession* session)
{
    std::lock_guard guard(mutex_);
    sessions_.erase(session);
    ADAPTIV_DEBUG_CERR("left(id" << session << ')');
}

void SharedState::push(std::string message)
{
    // Put a message in a shared pointer so we can use it in multiple queues
    auto const messageSPtr =
        std::make_shared<std::string const>(std::move(message));

    // Flush the queue by writing its contents to the underlying websocket
    for (auto session : sessions_) {
        session->queue_.push(messageSPtr);
        net::spawn(session->websocket_.get_executor(),
            [session](net::yield_context yield)
            {
                session->write(yield);
            });
    }
}

void SharedState::enqueue(std::string message)
{
    // Put a message in a shared pointer so we can use it in multiple queues
    auto const messageSPtr =
               std::make_shared<std::string const>(std::move(message));

    std::lock_guard guard(mutex_);
    for (auto session : sessions_) {
        session->queue_.push(messageSPtr);
    }
}

void SharedState::isBusy(bool set)
{
    std::lock_guard guard(mutex_);
    isBusy_ = set;
}

bool SharedState::isBusy()
{
    std::lock_guard guard(mutex_);
    return isBusy_;
}

void SharedState::solve()
{
    // If the solver was running previously, join so that we can launch again
    if (!isBusy() && solverThread_.joinable()) {
        solverThread_.join();
    }

    isBusy_ = true;
    solverThread_ = std::thread(&solver::RANS::run, &ransSolver_);
}

std::shared_ptr<SharedState> SharedState::makeShared(
    net::io_context& context,
    std::string documentRoot)
{
    return std::make_shared<SharedState>(context, std::move(documentRoot));
}

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
