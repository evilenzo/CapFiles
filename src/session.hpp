/**
 * @file Contains main session declaration
 */

#pragma once

#include "cli_wrapper.hpp"
#include "download_manager.hpp"

#include <utility>

/// @brief Session class (Facade design pattern)
class Session {
    /// @brief Hardcoded host information for DownloadManager
    static constexpr DownloadManager::HostInfo HOST_INFO{
        "ftp.moex.com", "/pub/FAST/ASTS/dump/"};

  public:
    /**
     * Constructor with member initialization
     * @param prompt string for command-line prompt
     */
    Session(std::string prompt)
        : m_prompt(std::move(prompt)), m_cli_wrapper(m_prompt) {}

    /// @brief Initialize and start CLI instance
    void Start() {
        m_cli_wrapper.AddMenuEntries(CommandObject(
            "get", "Get file from server by filename",
            [&](std::ostream& os, const std::string& filename) {
                os << "Downloading..." << std::endl;
                try {
                    m_download_manager.GetFile(filename);
                } catch (const boost::beast::system_error& ex) {
                    os << ex.what() << std::endl << ex.code() << std::endl;
                }
            }));

        m_cli_wrapper.Start();
    }

  private:
    /// @brief String for command-line prompt
    std::string m_prompt;

    /// @brief CliWrapper module
    CliWrapper m_cli_wrapper;

    /// @brief DownloadManager module
    DownloadManager m_download_manager{HOST_INFO};
};