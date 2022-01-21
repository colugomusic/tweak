#pragma once

#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak {
namespace std {
namespace speed {

constexpr auto FREEZE = 0.0f;
constexpr auto THIRTYSECOND = 0.03125f;
constexpr auto SIXTEENTH = 0.0625f;
constexpr auto EIGHTH = 0.125f;
constexpr auto QUARTER = 0.25f;
constexpr auto HALF = 0.5f;
constexpr auto NORMAL = 1.0f;
constexpr auto DOUBLE = 2.0f;
constexpr auto TRIPLE = 3.0f;

inline auto constrain(float v)
{
    if (v < convert::linear_to_speed(-32.0f)) return FREEZE;
    if (v > 32.0f) return 32.0f;

    return v;
};

inline auto increment(float v, bool precise)
{
    if (v <= FREEZE) return convert::linear_to_speed(-32.0f);

    return constrain(convert::linear_to_speed(tweak::increment<1, 10>(convert::speed_to_linear(v), precise)));
};

inline auto decrement(float v, bool precise)
{
    return constrain(convert::linear_to_speed(tweak::decrement<1, 10>(convert::speed_to_linear(v), precise)));
};

inline auto drag(float v, int amount, bool precise) -> float
{
    if (v <= FREEZE) v = convert::linear_to_speed(-32.0f);

    return constrain(convert::linear_to_speed(tweak::drag<float, 1, 10>(convert::speed_to_linear(v), amount / 5, precise)));
};

inline auto from_string(const ::std::string& str) -> ::std::optional<float>
{
    ::std::string uppercase = str;

    ::std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

    if (uppercase.find("FREEZE") != ::std::string::npos) return FREEZE;
    if (uppercase.find("NORMAL") != ::std::string::npos) return NORMAL;
    if (uppercase.find("DOUBLE") != ::std::string::npos) return DOUBLE;
    if (uppercase.find("TRIPLE") != ::std::string::npos) return TRIPLE;

    ::std::regex r("1/([\\d]+)");
    ::std::smatch match;

    if (::std::regex_search(str, match, r))
    {
        return 1.0f / ::std::stoi(match[1]);
    }

    auto ff = tweak::find_number<float>(str);

    if (!ff) return ff;

    return *ff;
};

inline auto to_string(float v)
{
	constexpr auto THRESHOLD { 0.001f };

    const auto milestone_hit = [THRESHOLD](float value, float milestone)
    {
        return value > milestone - THRESHOLD && value < milestone + THRESHOLD;
    };

    ::std::stringstream ss;

    if (v <= FREEZE)
    {
        ss << "Freeze";
    }
    else if (v < 1.0f - THRESHOLD)
    {
        const auto recip = 1.0f / v;
        const auto rounded_recip = ::std::round(recip);

        if (::std::abs(recip - rounded_recip) < THRESHOLD)
        {
            ss << "1/" << rounded_recip;
        }
        else
        {
            ss << "x" << v;
        }
    }
    else if (milestone_hit(v, NORMAL))
    {
        ss << "Normal";
    }
    else if (milestone_hit(v, DOUBLE))
    {
        ss << "Double";
    }
    else if (milestone_hit(v, TRIPLE))
    {
        ss << "Triple";
    }
    else
    {
        ss << "x" << v;
    }

    return ss.str();
}

} // speed
} // std
} // tweak