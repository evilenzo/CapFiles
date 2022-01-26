#pragma once

#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <cli/filehistorystorage.h>

#include <concepts>
#include <iostream>
#include <vector>

template <typename T>
concept CommandT = requires(T object) {
    { object.name } -> std::same_as<const char*&>;
    { object.comment } -> std::same_as<const char*&>;
    object.function;
};

template <typename Func>
struct CommandObject {
    using string_t = const char*;

    CommandObject(string_t name, string_t comment, Func function)
        : name(name), comment(comment), function(function) {}

    string_t name;
    string_t comment;
    Func function;
};

class CliWrapper {
  public:
    CliWrapper(std::string_view prompt) : m_prompt(prompt) {}

    template <CommandT... Args>
    void AddMenuEntries(Args&&... args) {
        m_menu = std::make_unique<cli::Menu>(std::string(m_prompt));

        std::apply(
            [&](const auto&... i) {
                ((m_menu->Insert(i.name, i.function, i.comment)), ...);
            },
            std::forward_as_tuple(args...));
    }

    void Start() {
        cli::Cli cli(std::move(m_menu));
        cli::CliFileSession session(cli);
        session.Start();
    }


  private:
    std::string_view m_prompt;
    std::unique_ptr<cli::Menu> m_menu;
};