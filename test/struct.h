#pragma once

#include <format>
#include <string>

struct Struct {
    std::string s_;
    int a_;
    Struct(const std::string& s, int a) : s_(s), a_(a) {}
};

template <>
struct std::formatter<Struct> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Struct& obj, std::format_context& ctx) const {
        // 双花括号进行转义
        return std::format_to(ctx.out(), "{{ s:\"{}\", a:{} }}", obj.s_, obj.a_);
    }
};
