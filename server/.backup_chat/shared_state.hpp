/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SHARED_STATE_HPP
#define ADAPTIV_SHARED_STATE_HPP

#include <unordered_set>
#include <memory>

#include <adaptiv/net/net.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

class WebSocketSession;

/**
 * Represents the shared server state - information that every object in the
 * system needs to have access to
 */
class SharedState
{
    /// Also an http server that serves html files, etc
    std::string documentRoot_;

    /// Keep a list of active websocket sessions (works with implicit strand)
    std::unordered_set<WebSocketSession*> sessions_;
public:
    explicit SharedState(std::string documentRoot);

    void join  (WebSocketSession* session); ///< Add a new session
    void leave (WebSocketSession* session); ///< Remove a session

    /**
     * Pushes a message to the queue of each active session and then flushes
     * the queue buy writing all outgoing messages to the underlying websocket.
     */
    void pushToAll(std::string message);

    std::string const& documentRoot() const noexcept { return documentRoot_; }

    // We'll use shared_ptr to manage the shared state (do we really?)
    /// Convenience function - make a shared_ptr
    static std::shared_ptr<SharedState> makeShared(std::string documentRoot);
};

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_SHARED_STATE_HPP
