#pragma once

#include <optional>
#include <sstream>
#include <string>
#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak {
namespace std {
namespace percentage {

inline auto stepify(float v)
{
	return tweak::math::stepify<1000>(v);
}

inline auto constrain(float v)
{
	return ::std::clamp(v, 0.0f, 1.0f);
};

inline auto increment(float v, bool precise)
{
	return tweak::increment<100, 1000>(v, precise);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<100, 1000>(v, precise);
};

inline auto drag(float v, int amount, bool precise)
{
	return tweak::drag<float, 100, 1000>(v, amount / 5, precise);
};

inline auto to_string(float v)
{
	::std::stringstream ss;

	ss << stepify(v * 100.0f) << "%";

	return ss.str();
}

inline auto from_string(const ::std::string& str) -> ::std::optional<float>
{
	auto value = tweak::find_number<float>(str);

	if (!value) return ::std::optional<float>();

	return (*value / 100.0f);
};

namespace bipolar {

inline auto constrain(float v)
{
	return ::std::clamp(v, -1.0f, 1.0f);
};

} // bipolar
} // percentage
} // std
} // tweak