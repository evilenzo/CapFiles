/**
 * @file Contains main session declaration
 */

#pragma once

#include "cli_wrapper.hpp"
#include "download_manager.hpp"
#include "archiver.hpp"

#include <array>
#include <utility>

/// @brief Session class (Facade design pattern)
class Session {
  public:
    /**
     * Constructor with member initialization
     * @param prompt string for command-line prompt
     */
    Session(std::string prompt)
        : m_prompt(std::move(prompt)), m_cli_wrapper(m_prompt) {}

    /// @brief Initialize and start CLI instance
    void Start() {
        m_cli_wrapper.CreateMenu(m_prompt);
        std::apply(
            [&](auto&&... module) {
                ((m_cli_wrapper.AddMenuEntries(module.METHODS)), ...);
            },
            m_modules);
        m_cli_wrapper.Start();
    }

  private:
    /// @brief String for command-line prompt
    std::string m_prompt;

    /// @brief CliWrapper module
    CliWrapper m_cli_wrapper;

    static constexpr auto m_modules = std::tuple<DownloadManager, Archiver>();
};