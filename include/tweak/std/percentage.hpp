#pragma once

#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak::std_::percentage {

template <std::floating_point T> [[nodiscard]] constexpr
auto stepify(T v) -> T {
	return tweak::math::stepify<1000>(v);
}

template <std::floating_point T> [[nodiscard]] constexpr
auto constrain(T v) -> T {
	return ::std::clamp(v, T(0), T(1));
};

template <std::floating_point T> [[nodiscard]] constexpr
auto increment(T v, bool precise) -> T {
	return tweak::increment<100, 1000>(v, precise);
};

template <std::floating_point T> [[nodiscard]] constexpr
auto decrement(T v, bool precise) -> T {
	return tweak::decrement<100, 1000>(v, precise);
};

template <std::floating_point T> [[nodiscard]] constexpr
auto drag(T v, int amount, bool precise) -> T {
	return tweak::drag<float, 100, 1000>(v, amount / 5, precise);
};

template <std::floating_point T> [[nodiscard]]
auto to_string(T v) -> std::string {
	auto ss = std::stringstream{};
	ss << stepify(v * T(100)) << "%";
	return ss.str();
}

template <std::floating_point T = float> [[nodiscard]]
auto from_string(const std::string& str) -> std::optional<T> {
	auto value = tweak::find_number<float>(str);
	if (!value) { return std::nullopt; }
	else        { return (*value / T(100)); }
};

} // tweak::std_::percentage

namespace tweak::std_::percentage::bipolar {

template <std::floating_point T> [[nodiscard]] constexpr
auto constrain(T v) -> T {
	return std::clamp(v, T(-1), T(1));
};

} // tweak::std_::percentage::bipolar

