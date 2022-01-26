#pragma once

#include "command.hpp"

#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <cli/filehistorystorage.h>

#include <concepts>
#include <iostream>
#include <vector>

/// @brief Wrapper around cli::cli
class CliWrapper {
  public:
    /**
     * @brief Create new menu
     * @details Every time you call Start() you need to recreate menu for
     * another Start() call because menu is non-copyable
     */
    void CreateMenu(std::string_view prompt) {
        m_menu = std::make_unique<cli::Menu>(prompt.data());
    }

    /// @brief Add commands from tuple
    template <CommandT... Args>
    void AddMenuEntries(std::tuple<Args...> commands) {
        std::apply(
            [&](const auto&... i) {
                ((m_menu->Insert(i.name, i.function, i.comment)), ...);
            },
            commands);
    }

    /// @brief Start CLI session
    void Start() {
        cli::Cli cli(std::move(m_menu));
        cli::CliFileSession session(cli);
        session.Start();
    }


  private:
    /// @brief Menu member
    std::unique_ptr<cli::Menu> m_menu;
};