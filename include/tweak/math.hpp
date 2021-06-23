#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>

namespace tweak {
namespace math {

template <class T>
constexpr T lerp(T a, T b, T x)
{
	return (x * (b - a)) + a;
}

template <class T>
constexpr T inverse_lerp(T a, T b, T x)
{
	return (x - a) / (b - a);
}

template <class T>
constexpr T stepify(T value, T step)
{
	if (step != 0)
	{
		value = ::std::floor(value / step + T(0.5)) * step;
	}

	return value;
}

template <int N, class T>
constexpr T stepify(T v)
{
	return stepify(v, T(1.0) / N);
}

} // math
} // tweak
