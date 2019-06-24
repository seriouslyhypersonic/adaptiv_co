/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_REQUEST_HPP
#define ADAPTIV_REQUEST_HPP

#include <adaptiv/cloud/protocol/network_exchange.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

/**
 * An adaptiv client-server network Request
 * @tparam NetworkMessage A JSON serializable type holding the message to
 * exchange int the Request.
 * @note To ensure your NetworkMessage models the JSON serializable concept,
 * add serialization methods using one of cereal's facilities. E.g.
 * @code
 *     template<class Archive>
 *     void serialize(Archive& archive)
 *     {
 *          archive(member1, member2, ...);
 *     }
 * @endcode
 */
template<class NetworkMessage>
class Request: public NetworkExchange<NetworkMessage>
{
public:
    /**
     * Create a Request from data to be sent
     * @param target The target of the request
     * @param message The message (i.e. params) sent to the request \c target
     */
    Request(std::string const& target, NetworkMessage const& message)
    : NetworkExchange<NetworkMessage>::NetworkExchange(target, message)
    { /* Invoke the base constructor to enable template type deduction */ }

    /// Reconstruct a Request from received data
    explicit Request(std::istream& request)
    : NetworkExchange<NetworkMessage>::NetworkExchange(request)
    { }

    /// Make the Request serializable
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<NetworkExchange<NetworkMessage>>(this));
    }

    /// The serialized Request in JSON format
    std::string json()
    {
        return NetworkExchange<NetworkMessage>::json("request");
    }
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_REQUEST_HPP
