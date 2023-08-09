#pragma once

#include "math.hpp"

namespace tweak {
#include "third_party/const_math.hpp"
namespace convert {

template <class T>
inline constexpr T linear_to_ratio(T x, T max = T(100))
{
	if (x <= 0) return 1.0f;

	return ::std::pow(max, ::std::pow(x, T(2)));
}

template <class T>
inline constexpr T ratio_to_linear(T x, T max = T(100))
{
	if (x <= 1) return 0.0f;

	return ::std::sqrt(::std::log(x)) / ::std::sqrt(::std::log(max));
}

template <class T>
inline T bi_to_uni(T bi)
{
	return (bi + T(1)) / T(2);
}

template <class T>
inline T uni_to_bi(T uni)
{
	return (uni * T(2)) - T(1);
}

template <class T>
inline T pitch_to_frequency(T pitch)
{
	return T(8.1758) * ::std::pow(T(2), pitch / T(12));
}

template <class T>
inline float frequency_to_pitch(T frequency)
{
	return T(12) * (::std::log(frequency / T(8.1758)) / ::std::log(T(2)));
}

template <class T>
inline T linear_to_filter_hz(T linear)
{
	return pitch_to_frequency(lerp(T(-8.513f), T(135.076f), linear));
}

template <class T>
inline T filter_hz_to_linear(T hz)
{
	return inverse_lerp(T(-8.513f), T(135.076f), frequency_to_pitch(hz));
}

template <class T>
constexpr inline T linear_to_db(T linear) {
	return T(tweak::const_math::log(linear)) * T(8.6858896380650365530225783783321);
}

template <class T>
constexpr inline T db_to_linear(T db) {
	return T(tweak::const_math::exp(db * T(0.11512925464970228420089957273422)));
}

template <class T>
constexpr inline T linear_to_speed(T linear)
{
	return ::std::pow(T(0.5), -linear);
}

template <class T>
constexpr inline T speed_to_linear(T speed)
{
	return ::std::log(speed) / ::std::log(T(2));
}

template <class T>
T p_to_ff(T p)
{
	return ::std::pow(T(2), p / T(12));
}

template <class T>
T ff_to_p(T ff)
{
	return (::std::log(ff) / ::std::log(T(2))) * T(12);
}

} // convert
} // tweak