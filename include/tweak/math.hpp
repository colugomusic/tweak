#pragma once

#include "const-math.hpp"

namespace tweak::math {

template <class T> [[nodiscard]] constexpr
auto lerp(T a, T b, T x) -> T {
	return (x * (b - a)) + a;
}

template <class T> [[nodiscard]] constexpr
auto inverse_lerp(T a, T b, T x) -> T {
	return (x - a) / (b - a);
}

template <class T> [[nodiscard]] constexpr
auto stepify(T value, T step) -> T {
	if (step != 0) {
		value = const_math::floor(value / step + T(0.5)) * step;
	}
	return value;
}

template <int N, class T> [[nodiscard]] constexpr
auto stepify(T v) -> T {
	return stepify(v, T(1.0) / N);
}

} // tweak::math
