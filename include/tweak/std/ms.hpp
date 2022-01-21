#pragma once

#include <optional>
#include <sstream>
#include <string>
#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak {
namespace std {
namespace ms {

inline auto stepify(float v)
{
	return math::stepify(v, 0.001f);
}

inline auto to_string(float v)
{
	::std::stringstream ss;

	ss << ms::stepify(v) << " ms";

	return ss.str();
}

inline auto from_string(const ::std::string& str)
{
	return tweak::find_positive_number<float>(str);
};

} // ms
} // std
} // tweak