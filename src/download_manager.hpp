/**
 * @file Contains DownloadManager declaration
 */

#pragma once

#include "command.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <fstream>
#include <string_view>
#include <tuple>

namespace Network {

/// @brief Struct containing host connection information
struct HostInfo {
    using string_t = const char*;
    string_t host;
    string_t endpoint;
    string_t service = "http";  // Protocol or port
};

}  // namespace Network

/// @brief Class providing functionality for getting and unpacking file(s) from
/// remote host
class DownloadManager {
    /// @brief HTTP version for request
    static constexpr auto HTTP_VERSION{11};

    /// @brief Hardcoded host information
    static constexpr Network::HostInfo HOST_INFO{"ftp.moex.com",
                                                 "/pub/FAST/ASTS/dump/"};

  public:
    /// @brief Get file from remote host
    static void GetFile(std::string_view filename);

    /// @brief Commands list for CLI wrapper
    static constexpr auto METHODS = std::tuple(  //
        CommandObject{"get", "Get file from server by filename",
                      [](std::ostream& os, const std::string& filename) {
                          os << "Downloading..." << std::endl;
                          try {
                              DownloadManager::GetFile(filename);
                          } catch (const boost::beast::system_error& ex) {
                              os << ex.what() << std::endl
                                 << ex.code() << std::endl;
                          }
                      }});
};