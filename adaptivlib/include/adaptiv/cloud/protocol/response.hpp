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
#include <adaptiv/traits/traits.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

namespace traits {
namespace detail {
template<class T>
using has_response_error_t = decltype(std::declval<T>().error);
} // namespace detail

/// Detect if type \c T has a std::string data member named 'error'
template<class T>
constexpr bool has_response_error_v =
    adaptiv::traits::is_detected_exact_v<std::string
                                        ,detail::has_response_error_t
                                        , T>;
} // namespace traits

/// Static assertion: assert type \c T has a response error data member
#define ADAPTIV_ASSERT_HAS_RESPONSE_ERROR(T)                                  \
static_assert(traits::has_response_error_v<T>,                                \
    "[adaptiv::cloud::protocol::Response] an adaptiv Response NetworkMessage "\
    "requires a std::string data member named 'error'")                       \

/**
 * An adaptiv client-server network Response
 * @tparam NetworkMessage A JSON serializable type holding the message to
 * exchange in the Response.
 * @note To ensure your NetworkMessage models the JSON serializable concept,
 * add serialization methods using one of cereal's facilities. E.g.
 * @code
 *     template<class Archive>
 *     void serialize(Archive& archive)
 *     {
 *          archive(member1, member2, ...);
 *     }
 * @endcode
 * @note All Response NetworkMessages need a \c std::string data member named
 * 'error' for error handling.
 */
template<class NetworkMessage>
class Response: public NetworkExchange<NetworkMessage>
{
public:
    /**
     * Create a Response from data to be sent
     * @param target The target of the response
     * @param message The message (i.e. results) sent by the response \c target
     */
    Response(std::string const& target, NetworkMessage const& message)
    : NetworkExchange<NetworkMessage>::NetworkExchange(target, message)
    {
        /* Invoke the base constructor to enable template type deduction */
        ADAPTIV_ASSERT_HAS_RESPONSE_ERROR(NetworkMessage);
    }

    /// Reconstruct a Response from received data
    explicit Response(std::istream& request)
    : NetworkExchange<NetworkMessage>::NetworkExchange(request)
    {
        ADAPTIV_ASSERT_HAS_RESPONSE_ERROR(NetworkMessage);
    }

    /// Make the Request serializable
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<NetworkExchange<NetworkMessage>>(this));
    }

    /// The serialized Response in JSON format
    std::string json()
    {
        return NetworkExchange<NetworkMessage>::json("response");
    }
};

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_RESPONSE_HPP
