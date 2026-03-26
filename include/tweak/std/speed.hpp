#pragma once

#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak::std_::speed {

constexpr auto FREEZE       = 0.0f;
constexpr auto THIRTYSECOND = 0.03125f;
constexpr auto SIXTEENTH    = 0.0625f;
constexpr auto EIGHTH       = 0.125f;
constexpr auto QUARTER      = 0.25f;
constexpr auto HALF         = 0.5f;
constexpr auto NORMAL       = 1.0f;
constexpr auto DOUBLE       = 2.0f;
constexpr auto TRIPLE       = 3.0f;

template <std::floating_point T> [[nodiscard]] constexpr
auto constrain(T v) -> T {
         if (v < convert::linear_to_speed(T(-32))) { return FREEZE; }
    else if (v > T(32))                            { return T(32); }
    else                                           { return v; }
};

template <std::floating_point T> [[nodiscard]] constexpr
auto increment(T v, bool precise) -> T {
    if (v <= FREEZE) { return convert::linear_to_speed(T(-32)); }
    else             { return constrain(convert::linear_to_speed(tweak::increment<1, 10>(convert::speed_to_linear(v), precise))); }
};

template <std::floating_point T> [[nodiscard]] constexpr
auto decrement(T v, bool precise) -> T {
    return constrain(convert::linear_to_speed(tweak::decrement<1, 10>(convert::speed_to_linear(v), precise)));
};

template <std::floating_point T> [[nodiscard]] constexpr
auto drag(T v, int amount, bool precise) -> T {
    if (v <= FREEZE) { v = convert::linear_to_speed(T(-32)); }
    return constrain(convert::linear_to_speed(tweak::drag<float, 1, 10>(convert::speed_to_linear(v), amount / 5, precise)));
};

template <std::floating_point T = float> [[nodiscard]]
auto from_string(const std::string& str) -> std::optional<T> {
    auto uppercase = str;
    std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);
    if (uppercase.find("FREEZE") != std::string::npos) { return FREEZE; }
    if (uppercase.find("NORMAL") != std::string::npos) { return NORMAL; }
    if (uppercase.find("DOUBLE") != std::string::npos) { return DOUBLE; }
    if (uppercase.find("TRIPLE") != std::string::npos) { return TRIPLE; }
    auto r     = std::regex{"1/([\\d]+)"};
    auto match = std::smatch{};
    if (std::regex_search(str, match, r)) {
        return T(1) / std::stoi(match[1]);
    }
    auto ff = tweak::find_number<float>(str);
    if (!ff) { return ff; }
    return *ff;
};

template <std::floating_point T> [[nodiscard]]
auto to_string(T v) -> std::string {
	constexpr auto THRESHOLD = T(0.001);
    const auto milestone_hit = [THRESHOLD](T value, T milestone) {
        return value > milestone - THRESHOLD && value < milestone + THRESHOLD;
    };
    auto ss = std::stringstream{};
    if (v <= FREEZE) {
        ss << "Freeze";
    }
    else if (v < T(1) - THRESHOLD) {
        const auto recip         = T(1) / v;
        const auto rounded_recip = std::round(recip);
        if (std::abs(recip - rounded_recip) < THRESHOLD) { ss << "1/" << rounded_recip; }
        else                                             { ss << "x" << v; }
    }
    else if (milestone_hit(v, NORMAL)) {
        ss << "Normal";
    }
    else if (milestone_hit(v, DOUBLE)) {
        ss << "Double";
    }
    else if (milestone_hit(v, TRIPLE)) {
        ss << "Triple";
    }
    else {
        ss << "x" << v;
    }
    return ss.str();
}

} // tweak::std_::speed
