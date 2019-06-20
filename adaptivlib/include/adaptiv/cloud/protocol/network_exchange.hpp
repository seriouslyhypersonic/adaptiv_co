/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_NETWORK_EXCHANGE_HPP
#define ADAPTIV_NETWORK_EXCHANGE_HPP

#include <type_traits>
#include <string>
#include <utility>
#include <iostream>
#include <string>
#include <sstream>

#include <adaptiv/macros.hpp>
#include <adaptiv/serialization/external/cereal/details/traits.hpp>
#include <adaptiv/serialization/external/cereal/archives/json.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

namespace traits {
template<class T>
bool constexpr is_json_serializable_v =
    std::conjunction_v
        <cereal::traits::is_input_serializable<T, cereal::JSONInputArchive>
        ,cereal::traits::is_output_serializable<T, cereal::JSONOutputArchive>>;
} // namespace traits

/**
 * The base class for an adaptiv client-server network exchange
 * @tparam Message A JSON serializable type holding the message to exchange
 * when sending a request/response;
 */
template<class Message>
class NetworkExchange
{
public:
    /**
     * Create a NetworkExchange from data to be sent
     * @param target The target of the exchange
     * @param message The message sent to or received from the \c target
     */
    NetworkExchange(std::string target, Message const& message)
    : target_(std::move(target))
    , message_(message)
    {
        static_assert(
            traits::is_json_serializable_v<Message>,
            "[adaptiv::cloud::protocol::NetworkExchange] "
            "a request/response message must be JSON serializable");
    }

    /// Reconstruct a NetworkExchange from received data
    explicit NetworkExchange(std::istream& request)
    {
        NetworkExchange temp("", Message{});
        {
            // Load the data into temp
            cereal::JSONInputArchive iarchive(request);
            iarchive(temp);
        }
        *this = temp;
    }

    /// Make the NetworkExchange serializable
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
        cereal::make_nvp("target", target_),
        cereal::make_nvp("message", message_));
    }

    std::string const& target()  const noexcept { return target_; }
    Message     const& message() const noexcept { return message_; };

protected:
    /** The serialized NetworkExchange in JSON format
     * @param exchangeType The type of the exchange (i.e. "request"/"response")
     */
    std::string json(std::string const& exchangeType)
    {
        std::ostringstream out;
        {
            cereal::JSONOutputArchive oarchive(out);
            oarchive(cereal::make_nvp(exchangeType, *this));
        }
        return out.str();
    }

    std::string target_;
    Message     message_;
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_NETWORK_EXCHANGE_HPP
