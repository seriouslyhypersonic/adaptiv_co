/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <iostream>
#include <type_traits>
#include <vector>
#include <stdexcept>

#include "server.hpp"
#include "shared_state.hpp"
#include "websocket_session.hpp"

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_CLOUD_NAMESPACE_BEGIN
ADAPTIV_SERVER_NAMESPACE_BEGIN

namespace detail {

/// Return a reasonable mim type based on the extension of a file
beast::string_view mimeType(beast::string_view path)
{
    using boost::beast::iequals;
    auto const extension = [&path]
    {
        auto const pos = path.rfind(".");
        if (pos == beast::string_view::npos) {
            return beast::string_view{};
        }
        return path.substr(pos);
    }();

    if(iequals(extension, ".htm"))  return "text/html";
    if(iequals(extension, ".html")) return "text/html";
    if(iequals(extension, ".php"))  return "text/html";
    if(iequals(extension, ".css"))  return "text/css";
    if(iequals(extension, ".txt"))  return "text/plain";
    if(iequals(extension, ".js"))   return "application/javascript";
    if(iequals(extension, ".json")) return "application/json";
    if(iequals(extension, ".xml"))  return "application/xml";
    if(iequals(extension, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(extension, ".flv"))  return "video/x-flv";
    if(iequals(extension, ".png"))  return "image/png";
    if(iequals(extension, ".jpe"))  return "image/jpeg";
    if(iequals(extension, ".jpeg")) return "image/jpeg";
    if(iequals(extension, ".jpg"))  return "image/jpeg";
    if(iequals(extension, ".gif"))  return "image/gif";
    if(iequals(extension, ".bmp"))  return "image/bmp";
    if(iequals(extension, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(extension, ".tiff")) return "image/tiff";
    if(iequals(extension, ".tif"))  return "image/tiff";
    if(iequals(extension, ".svg"))  return "image/svg+xml";
    if(iequals(extension, ".svgz")) return "image/svg+xml";
    return "application/text";
}

/// Append an HTTP rel-path to a local filesystem path.
std::string pathConcatenate(beast::string_view base, beast::string_view path)
{
    if (base.empty()) {
        return path.to_string();
    }
    std::string result = base.to_string();

    // Platform separator
#if BOOST_MSVC
    char constexpr pathSeparator = '\\';
#else
    char constexpr pathSeparator = '/';
#endif
    // Append HTTP path to local filesystem path
    if (result.back() == pathSeparator) {
        result.resize(result.size()-1);
    }
    result.append(path.data(), path.size());

#if BOOST_MSVC
    // Windows: replace POSIX separators, if any.
    for (auto& c : result) {
        if (c == '/') {
            c = pathSeparator;
        }
    }
#endif
    return result;
}

/**
 * Produce an HTTP response for the given request. The type of he response
 * object depends on the contents of the request, so the interface requires the
 * caller to pass a generic lambda for receiving the response.
 */
template<class Body, class Allocator, class Send>
void handleRequest(
    beast::string_view documentRoot,
    beast::http::request<Body, beast::http::basic_fields<Allocator>>&& request,
    Send&& send)
{
    // Returns a bad request response
    auto const badRequest =
        [&request](beast::string_view why)
        {
            beast::http::response<beast::http::string_body>
                response{beast::http::status::bad_request, request.version()};
            response.set(beast::http::field::server, ADAPTIV_VERSION_STRING);
            response.set(beast::http::field::content_type, "text/html");
            response.keep_alive(request.keep_alive());
            response.body() = std::string(why);
            response.prepare_payload();
            return response;
        };

    // Returns a not found response
    auto const notFound =
        [&request](beast::string_view target)
        {
            beast::http::response<beast::http::string_body>
                response{beast::http::status::not_found, request.version()};
            response.set(beast::http::field::server, ADAPTIV_VERSION_STRING);
            response.set(beast::http::field::content_type, "text/html");
            response.keep_alive(request.keep_alive());
            response.body() =
                "The resource '" + std::string(target) + "' was not found.";
            response.prepare_payload();
            return response;
        };

    // Returns a server error response
    auto const serverError =
        [&request](beast::string_view what)
        {
            beast::http::response<beast::http::string_body>
                response{beast::http::status::internal_server_error,
                         request.version()};
            response.set(beast::http::field::server, ADAPTIV_VERSION_STRING);
            response.set(beast::http::field::content_type, "text/html");
            response.keep_alive(request.keep_alive());
            response.body() = "An error occurred: '" + std::string(what) + "'";
            response.prepare_payload();
            return response;
        };

    // Make sure we can handle the method
    if (request.method() != beast::http::verb::get &&
        request.method() != beast::http::verb::head) {
        return send(badRequest("Unknown HTTP-method"));
    }

    // Request path must be absolute and not contain "..".
    if (request.target().empty() ||
        request.target()[0] != '/' ||
        request.target().find("..") != beast::string_view::npos) {
        return send(badRequest("Illegal request-target"));
    }

    // Build the path to the requested file
    std::string path = detail::pathConcatenate(documentRoot, request.target());
    if (request.target().back() == '/') path.append("index.html");

    // Attempt to open the file
    boost::beast::error_code ec;
    beast::http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if (ec == boost::system::errc::no_such_file_or_directory) {
        return send(notFound(request.target()));
    }

    // Handle an unknown error
    if (ec) return send(serverError(ec.message()));

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if (request.method() == beast::http::verb::head) {
        beast::http::response<beast::http::empty_body>
            response{beast::http::status::ok, request.version()};
        response.set(beast::http::field::server, ADAPTIV_VERSION_STRING);
        response.set(beast::http::field::content_type, mimeType(path));
        response.content_length(size);
        response.keep_alive(request.keep_alive());
        return send(std::move(response));
    }

    // Respond to GET request
    beast::http::response<beast::http::file_body>
        response{std::piecewise_construct,
                 std::make_tuple(std::move(body)),
                 std::make_tuple(beast::http::status::ok, request.version())};
    response.set(beast::http::field::server, ADAPTIV_VERSION_STRING);
    response.set(beast::http::field::content_type, mimeType(path));
    response.content_length(size);
    response.keep_alive(request.keep_alive());

    std::cout << "[sent] " << request.target() << '\n'; // For debugging

    return send(std::move(response));
}

/// Handles a WebSocketSession
template<class Body, class Allocator>
void doWebSocketSession(
    net::tcp::socket&& socket,
    std::shared_ptr<SharedState> const& state,
    beast::http::request<Body
                        ,boost::beast::http::basic_fields<Allocator>> request,
    net::yield_context yield)
{
    // Create a WebSocketSession and accept the handshake
    auto session = WebSocketSession::makeShared(std::move(socket), state);
    auto ec = session->accept(request, yield);
    if (ec) return fail(ec, "websocket accept");

    bool hasConnection = true;

    while (hasConnection) {
        session->read(hasConnection, yield);
    }
}

/// Handles an HTTP server connection
void doHttpSession(
    beast::tcp_stream&& stream,
    std::shared_ptr<SharedState> const& state,
    net::yield_context yield)
{
    bool close = false;
    error_code ec;

    // This buffer is required to persist across reads
    beast::flat_buffer buffer;

    while (true) {
        // Set the timeout
        stream.expires_after(std::chrono::seconds(30));

        // Read a request
        beast::http::request<beast::http::string_body> request;
        beast::http::async_read(stream, buffer, request, yield[ec]);
        if (ec == beast::http::error::end_of_stream) break; // They closed
        if (ec) return fail(ec, "read");

        // --- WebSocket (check if it is an upgrade)
        if (beast::websocket::is_upgrade(request)) {
            net::spawn(
                stream.get_executor(),
                [&stream, &request, state](net::yield_context yield)
                {
                    detail::doWebSocketSession(
                        stream.release_socket(),
                        state,
                        std::move(request),
                        yield);
                });
            return;
        }

        // --- HTTP response
        handleRequest(state->documentRoot(), std::move(request),
            [&stream, &close, &ec, &yield](auto&& response)
            {
                // Determine if we need to close the connection after
                close = response.need_eof();

                // We need the serializer here because the serializer requires
                // a non-const file_body, and the message oriented version of
                // http::write only works with const messages.
                using response_type = typename std::decay_t<decltype(response)>;
                bool constexpr isRequest = response_type::is_request::value;
                using body_type = typename response_type::body_type;
                using fields_type = typename response_type::fields_type;

                beast::http::serializer<isRequest
                                       ,body_type
                                       ,fields_type> serial(response);
                beast::http::async_write(stream, serial, yield[ec]);
            });

        if (ec) return fail(ec, "write");

        if (close) {
            // This means we should close the connection, usally because the
            // response indicated the "Connection: close" semantic.
            break;
        }
    }

    // Send a TCP shutdown
    stream.socket().shutdown(net::tcp::socket::shutdown_send, ec);

    // At this point the connection close gracefully
}

} // namespace detail

void doListen(
    net::io_context& context,
    net::tcp::endpoint endpoint,
    std::shared_ptr<SharedState> const& state,
    boost::asio::yield_context yield)
{
    error_code ec;

    // Open the acceptor
    net::tcp::acceptor acceptor(context);
    acceptor.open(endpoint.protocol(), ec);
    if (ec) return fail(ec, "open");

    // Allow address reuse
    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) return fail(ec, "set_option");

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if (ec) return fail(ec, "bind");

    // Start listening for connections
    acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec) return fail(ec, "listen");

    while (true) {
        // The socket performs the I/O
        net::tcp::socket socket(context);

        // Accept the connection
        acceptor.async_accept(socket, yield[ec]);
        if (ec) return fail(ec, "accept");

        // Launch a new HTTP session
        net::spawn(
            acceptor.get_executor(),
            [&socket, state](net::yield_context yield)
            {
                detail::doHttpSession(
                    beast::tcp_stream(std::move(socket)),
                    state,
                    yield);
            });
    }
}

ADAPTIV_SERVER_NAMESPACE_END
ADAPTIV_CLOUD_NAMESPACE_END
ADAPTIV_NAMESPACE_END
