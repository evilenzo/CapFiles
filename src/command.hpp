#pragma once

#include <concepts>

/// @brief Concept checking if type satisfying Command traits
template <typename T>
concept CommandT = requires(T object) {
    { object.name } -> std::same_as<const char*&>;
    { object.comment } -> std::same_as<const char*&>;
    object.function;
};

/// @brief Command object for CliWrapper and METHODS of every class
template <typename Func>
struct CommandObject {
    using string_t = const char*;

    constexpr CommandObject(string_t name, string_t comment, Func function)
        : name(name), comment(comment), function(function) {}

    string_t name;
    string_t comment;
    Func function;
};