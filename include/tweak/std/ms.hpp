#pragma once

#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak::std_::ms {

template <std::floating_point T> [[nodiscard]] constexpr
auto stepify(T v) -> T {
	return math::stepify(v, T(0.001));
}

template <std::floating_point T> [[nodiscard]]
auto to_string(T v) -> std::string {
	auto ss = std::stringstream{};
	ss << ms::stepify(v) << " ms";
	return ss.str();
}

template <std::floating_point T = float> [[nodiscard]]
auto from_string(const std::string& str) -> T {
	return tweak::find_positive_number<T>(str);
};

} // tweak::std_::ms
