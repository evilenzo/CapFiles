/**
 * @file Contains DownloadManager declaration
 */

#pragma once

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <fstream>
#include <string_view>

/// @brief Class providing functionality for getting and unpacking file(s) from
/// remote host
class DownloadManager {
    /// @brief HTTP version for request
    static constexpr auto HTTP_VERSION{11};

  public:
    /// @brief Struct for more clear constructor call
    struct HostInfo {
        std::string_view host;
        std::string_view endpoint;
        std::string_view service = "http";  // Protocol or port
    };

  public:
    DownloadManager(HostInfo info);

    void GetFile(std::string_view filename);
    static void Unpack() {}

  private:
    HostInfo m_host_info;
};