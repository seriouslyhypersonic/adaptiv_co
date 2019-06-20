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

// WIP
#include <thread>
#include <mutex>

#include <adaptiv/net/net.hpp>

#include "solver.hpp"

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
    net::io_context& context_;

    friend class solver::RANS;  ///< Solver needs access to enquee() and isBusy()
    solver::RANS ransSolver_;   ///< The solver (todo: ptr)
    std::thread solverThread_;  ///< A thread to run the solver

    /// Also an http server that serves html files, etc
    std::string documentRoot_;

    // [shared resources] ------------------------------------------------ begin
    /// Synchronizes access between the server and solver threads
    std::mutex mutex_;

    ///< The server is busy and the solver might be accessing shared resources
    bool isBusy_ = false;

    /// Keep a list of active websocket sessions (works with implicit strand)
    std::unordered_set<WebSocketSession*> sessions_;
    // [shared resources]  ------------------------------------------------- end

    // todo: replace friend class solver::RANS with friend functions

    /// Pushes a message to the queue of each active session \note Thread-safe
    void enqueue(std::string message);

    void isBusy(bool set);     ///< Setter \note Thread-safe

public:
    explicit SharedState(
        net::io_context& context,
        std::string documentRoot);

    ~SharedState();

    /// Add a new session \note Thread-safe
    void join  (WebSocketSession* session);

    /// Remove a session \note Thread-safe
    void leave (WebSocketSession* session);

    /**
     * todo: reimplement this function based on WebSocketSession::flush()
     * Pushes a message to the queue of each active session and then flushes
     * the queue by writing all outgoing messages to the underlying websocket.
     * @attention This function is \b NOT thread-safe and should only be used if
     * the solver is not running (i.e. no risk in accessing each session queue)
     */
    void push(std::string message);

    void solve(); ///< Start the solver

    bool isBusy(); ///< Getter \note Thread-safe

    std::mutex& mutex() { return mutex_; }

    std::string const& documentRoot() const noexcept { return documentRoot_; }

    // We'll use shared_ptr to manage the shared state (do we really?)
    /// Convenience function - make a shared_ptr
    static std::shared_ptr<SharedState> makeShared(
        net::io_context& context,
        std::string documentRoot);
};

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_SHARED_STATE_HPP
