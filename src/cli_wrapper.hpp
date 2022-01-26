#pragma once

#include "command.hpp"

#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <cli/filehistorystorage.h>

#include <concepts>
#include <iostream>
#include <vector>

class CliWrapper {
  public:
    CliWrapper(std::string_view prompt) {}

    void CreateMenu(std::string_view prompt) {
        m_menu = std::make_unique<cli::Menu>(prompt.data());
    }

    template <CommandT... Args>
    void AddMenuEntries(std::tuple<Args...> commands) {
        std::apply(
            [&](const auto&... i) {
                ((m_menu->Insert(i.name, i.function, i.comment)), ...);
            },
            commands);
    }

    void Start() {
        cli::Cli cli(std::move(m_menu));
        cli::CliFileSession session(cli);
        session.Start();
    }


  private:
    std::unique_ptr<cli::Menu> m_menu;
};