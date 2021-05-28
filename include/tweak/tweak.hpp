#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <optional>
#include <regex>
#include <sstream>
#include <string>

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
		value = std::floor(value / step + T(0.5)) * step;
	}

	return value;
}

template <int N, class T>
constexpr T stepify(T v)
{
	return stepify(v, T(1.0) / N);
}

} // math

template <class T> std::optional<T> find_number(const std::string& str);
template <class T> std::optional<T> find_positive_number(const std::string& str);

template <>
inline std::optional<float> find_number<float>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\.\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<float>();

	return std::stof(match[0].str());
}

template <>
inline std::optional<int> find_number<int>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <>
inline std::optional<float> find_positive_number<float>(const std::string& str)
{
	std::regex r("([\\.\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<float>();

	return std::stof(match[0].str());
}

template <>
inline std::optional<int> find_positive_number<int>(const std::string& str)
{
	std::regex r("([\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <int Normal, int Precise>
float increment(float v, bool precise)
{
	return v + 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise>
float decrement(float v, bool precise)
{
	return v - 1.0f / (precise ? Precise : Normal);
}

template <int Normal>
float increment(float v)
{
	return v + 1.0f / (Normal);
}

template <int Normal>
float decrement(float v)
{
	return v - 1.0f / (Normal);
}

template <int Normal, int Precise>
float drag(float v, int amount, bool precise)
{
	return v + (float(amount) / (precise ? Precise : Normal));
}

template <int Normal>
float drag(float v, int amount)
{
	return v + (float(amount) / Normal);
}

inline float constrain(float v, float min, float max)
{
	if (v < min) return min;
	if (v > max) return max;

	return v;
}

inline auto snap_value(float v, float step_size, float snap_amount)
{
	if (snap_amount <= 0.0f) return v;

	if (snap_amount >= 1.0f)
	{
		v /= step_size;
		v = std::round(v);
		v *= step_size;

		return v;
	}

	const auto up = std::ceil((v / step_size) + 0.0001f) * step_size;
	const auto down = std::floor(v / step_size) * step_size;
	const auto x = math::inverse_lerp(down, up, v);
	const auto t = x * 2.0f;
	const auto i = 1.0f + (std::pow(snap_amount, 4.0f) * 99.0f);
	const auto curve =
		t < 1.0f
		? 1.0f - (0.5f * (std::pow(1.0f - t, 1.0f / i) + 1.0f))
		: 0.5f * (std::pow(t - 1.0f, 1.0f / i) + 1.0f);

	return math::lerp(down, up, curve);
}

template <class T> using ToStringFunc = std::function<std::string(T)>;
template <class T> using FromStringFunc = std::function<std::optional<T>(const std::string&)>;
template <class T> using ConstrainFunc = std::function<T(T)>;
template <class T> using DragFunc = std::function<T(T, int, bool)>;
template <class T> using IncrementFunc = std::function<T(T, bool)>;
template <class T> using DecrementFunc = std::function<T(T, bool)>;
template <class T> using StepifyFunc = std::function<T(T)>;

template <class T>
struct Spec
{
	ToStringFunc<T> to_string;
	FromStringFunc<T> from_string;
	ConstrainFunc<T> constrain;
	DragFunc<T> drag;
	IncrementFunc<T> increment;
	DecrementFunc<T> decrement;
	StepifyFunc<T> stepify;
};

template <class T>
class Tweaker
{
public:

	Tweaker(const Spec<T>&& spec)
		: spec_(spec)
	{
	}

	auto stepify(float v)
	{
		return spec_.stepify(v);
	}

	auto constrain(float v)
	{
		return spec_.constrain(v);
	};

	auto snap_value(float v, float step_size, float snap_amount)
	{
		return stepify(snap_value(v, step_size, snap_amount));
	}

	auto increment(float v, bool precise)
	{
		return constrain(stepify(spec_.increment(v, precise)));
	}

	auto decrement(float v, bool precise)
	{
		return constrain(stepify(spec_.decrement(v, precise)));
	};

	auto drag(float v, int amount, bool precise)
	{
		return constrain(stepify(spec_.drag(v, amount, precise)));
	};

	auto to_string(float v)
	{
		return spec_.to_string(v);
	}

	auto from_string(const std::string& str)
	{
		return spec_.from_string(str);
	};

private:

	Spec<T> spec_;
};

} // tweak
