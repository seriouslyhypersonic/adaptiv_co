/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_RESPONSE_HPP
#define ADAPTIV_RESPONSE_HPP

#include <adaptiv/cloud/protocol/network_exchange.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

// An adaptiv response
template<class Message>
class Response: public NetworkExchange<Message>
{
public:
    /**
     * Create a Response from data to be sent
     * @param target The target of the response
     * @param message The message (i.e. results) sent by the response \c target
     */
    Response(std::string const& target, Message const& message)
    : NetworkExchange<Message>::NetworkExchange(target, message)
    { /* Invoke the base constructor to enable template type deduction */ }

    /// Reconstruct a Response from received data
    explicit Response(std::istream& request)
    : NetworkExchange<Message>::NetworkExchange(request)
    { }

    /// Make the Request serializable
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<NetworkExchange<Message>>(this));
    }

    /// The serialized Response in JSON format
    std::string json()
    {
        return NetworkExchange<Message>::json("response");
    }
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_RESPONSE_HPP
