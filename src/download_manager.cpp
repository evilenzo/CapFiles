#include "download_manager.hpp"

void DownloadManager::GetFile(std::string_view filename) {
    namespace beast = boost::beast;  // from <boost/beast.hpp>
    namespace http = beast::http;    // from <boost/beast/http.hpp>
    namespace asio = boost::asio;    // from <boost/asio.hpp>
    using tcp = asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

    // Error container
    beast::error_code ec;

    // The io_context is required for all I/O
    asio::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    const auto results =
        resolver.resolve(HOST_INFO.host, HOST_INFO.service, ec);

    if (ec) {
        throw beast::system_error{ec};
    }

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{
        http::verb::get,
        std::string(HOST_INFO.endpoint) + std::string(filename), HTTP_VERSION};
    req.set(http::field::host, HOST_INFO.host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response_parser<http::file_body> parser;
    parser.body_limit((std::numeric_limits<std::uint64_t>::max)());
    parser.get().body().open(filename.data(), boost::beast::file_mode::write,
                             ec);

    // Handle file creating errors
    if (ec) {
        throw beast::system_error{ec};
    }

    // Receive the HTTP response
    http::read(stream, buffer, parser);

    if (parser.get().result() != http::status::ok) {
        throw std::runtime_error("Error: " + std::to_string(static_cast<int>(
                                                 parser.get().result())));
    }

    parser.get().body().close();

    // Gracefully close the socket
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected sometimes happens
    if (ec && ec != beast::errc::not_connected) {
        throw beast::system_error{ec};
    }
}