/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "shared_state.hpp"

#include "shared_state.hpp"
#include "websocket_session.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

SharedState::SharedState(std::string documentRoot)
    : documentRoot_(std::move(documentRoot))
{ }

void SharedState::join(WebSocketSession* session)
{
    sessions_.insert(session);
}

void SharedState::leave(WebSocketSession* session)
{
    sessions_.erase(session);
}

void SharedState::pushToAll(std::string message)
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

std::shared_ptr<SharedState> SharedState::makeShared(std::string documentRoot)
{
    return std::make_shared<SharedState>(std::move(documentRoot));
}

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
