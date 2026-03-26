#pragma once

#include "const-math.hpp"
#include "math.hpp"

namespace tweak::convert {

template <class T> [[nodiscard]] constexpr
auto linear_to_ratio(T v, T max = T(100)) -> T {
	if (v <= 0) { return 1.0f; }
	else        { return const_math::pow(max, const_math::pow(v, T(2))); }
}

template <class T> [[nodiscard]] constexpr
auto ratio_to_linear(T v, T max = T(100)) -> T {
	if (v <= 1) { return 0.0f; }
	else        { return const_math::sqrt(const_math::log(v)) / const_math::sqrt(const_math::log(max)); }
}

template <class T> [[nodiscard]] constexpr
auto bi_to_uni(T v) {
	return (v + T(1)) / T(2);
}

template <class T> [[nodiscard]] constexpr
auto uni_to_bi(T v) {
	return (v * T(2)) - T(1);
}

template <class T> [[nodiscard]] constexpr
auto pitch_to_frequency(T v) -> T {
	return T(8.1758) * const_math::pow(T(2), v / T(12));
}

template <class T> [[nodiscard]] constexpr
auto frequency_to_pitch(T v) -> T {
	return T(12) * (const_math::log(v / T(8.1758)) / const_math::log(T(2)));
}

template <class T> [[nodiscard]] constexpr
auto linear_to_filter_hz(T v) -> T {
	return pitch_to_frequency(lerp(T(-8.513f), T(135.076f), v));
}

template <class T> [[nodiscard]] constexpr
auto filter_hz_to_linear(T v) -> T {
	return inverse_lerp(T(-8.513f), T(135.076f), frequency_to_pitch(v));
}

template <class T> [[nodiscard]] constexpr
auto linear_to_db(T v) -> T {
	return const_math::isfinite(v) ? T(const_math::log(v)) * T(8.6858896380650365530225783783321) : v;
}

template <class T> [[nodiscard]] constexpr
auto db_to_linear(T v) -> T {
	return const_math::isfinite(v) ? T(const_math::exp(v * T(0.11512925464970228420089957273422))) : v;
}

template <class T> [[nodiscard]] constexpr
auto linear_to_speed(T v) -> T {
	return const_math::pow(T(0.5), -v);
}

template <class T> [[nodiscard]] constexpr
auto speed_to_linear(T v) {
	return const_math::log(v) / const_math::log(T(2));
}

template <class T> [[nodiscard]] constexpr
auto p_to_ff(T p) -> T {
	return const_math::pow(T(2), p / T(12));
}

template <class T> [[nodiscard]] constexpr
auto ff_to_p(T ff) -> T {
	return (const_math::log(ff) / const_math::log(T(2))) * T(12);
}

} // tweak::convert
