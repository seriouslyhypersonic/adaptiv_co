/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <adaptiv/cloud/protocol/request.hpp>
#include <adaptiv/cloud/protocol/response.hpp>

#ifndef ADAPTIV_PROTOCOL_HPP
#define ADAPTIV_PROTOCOL_HPP

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_PROTOCOL_NAMESPACE_BEGIN

/** Get the target of a serialized NetworkExchange
 * This function is used to peek the serialized NetworkExchange, so that a
 * NetworkExchange object can be reconstructed with the appropriate Message
 * type.
 * @param networkExchange A JSON serialized NetworkExchange
 */
std::string target(std::string const& networkExchange)
{
    rapidjson::Document document;

    document.Parse(networkExchange.c_str());

    if (document.HasMember("request")) {
        // Handle a request
        if (!document["request"].HasMember("target")) return "";
        if (!document["request"]["target"].IsString()) return "";

        return document["request"]["target"].GetString();
    } else if (document.HasMember("response")) {
        // Handle a response
        if (!document["response"].HasMember("target")) return "";
        if (!document["response"]["target"].IsString()) return "";

        return document["response"]["target"].GetString();
    }

    // We didn't get a networkExchange
    return ""; //
}

ADAPTIV_PROTOCOL_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //ADAPTIV_PROTOCOL_HPP
