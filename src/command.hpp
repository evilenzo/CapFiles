#pragma once

#include <concepts>

template <typename T>
concept CommandT = requires(T object) {
    { object.name } -> std::same_as<const char*&>;
    { object.comment } -> std::same_as<const char*&>;
    object.function;
};

template <typename Func>
struct CommandObject {
    using string_t = const char*;

    constexpr CommandObject(string_t name, string_t comment, Func function)
        : name(name), comment(comment), function(function) {}

    string_t name;
    string_t comment;
    Func function;
};