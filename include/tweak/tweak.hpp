#pragma once

#include <functional>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include "math.hpp"

namespace tweak {

template <class T> std::optional<T> find_number(std::string str);
template <class T> std::optional<T> find_positive_number(std::string str);

template <>
inline std::optional<float> find_number<float>(std::string str) {
	std::regex r("(\\-?\\s*[\\.\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stof(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <>
inline std::optional<int> find_number<int>(std::string str) {
	std::regex r("(\\-?\\s*[\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stoi(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <>
inline std::optional<float> find_positive_number<float>(std::string str) {
	std::regex r("([\\.\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stof(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
}

template <>
inline std::optional<int> find_positive_number<int>(std::string str) {
	std::regex r("([\\d]+)");
	std::smatch match; 
	if (!std::regex_search(str, match, r)) {
		return std::nullopt;
	}
	try {
		return std::stoi(match[0].str());
	}
	catch (const std::exception&) {
		return std::nullopt;
	}
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

template <int Normal, int Precise>
int increment(int v, bool precise)
{
	return v + (precise ? Precise : Normal);
}

template <int Normal, int Precise>
int decrement(int v, bool precise)
{
	return v - (precise ? Precise : Normal);
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

template <int Normal>
int increment(int v)
{
	return v + Normal;
}

template <int Normal>
int decrement(int v)
{
	return v - Normal;
}

template <class T, int Normal, int Precise>
T drag(T v, int amount, bool precise)
{
	return v + T(float(amount) / (precise ? Precise : Normal));
}

template <class T, int Normal>
T drag(T v, int amount)
{
	return v + T(float(amount) / Normal);
}

template <class T>
inline T constrain(T v, T min, T max)
{
	if (v < min) return min;
	if (v > max) return max;

	return v;
}

template <class T>
inline ::std::string to_string(T v)
{
	::std::stringstream ss;

	ss << v;

	return ss.str();
}

inline auto snap_value(float v, float step_size, float snap_amount)
{
	if (step_size == 0.0f) return v;
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

template <class T> using ToStringFunc   = auto (*)(T) -> std::string;
template <class T> using FromStringFunc = auto (*)(std::string) -> T;
template <class T> using ConstrainFunc  = auto (*)(T) -> T;
template <class T> using DragFunc       = auto (*)(T, int, bool) -> T;
template <class T> using IncrementFunc  = auto (*)(T, bool) -> T;
template <class T> using DecrementFunc  = auto (*)(T, bool) -> T;
template <class T> using StepifyFunc    = auto (*)(T) -> T;

template <class T>
struct Spec {
	ToStringFunc<T> to_string;
	FromStringFunc<T> from_string;
	ConstrainFunc<T> constrain;
	DragFunc<T> drag;
	IncrementFunc<T> increment;
	DecrementFunc<T> decrement;
	StepifyFunc<T> stepify;
};

template <class T>
auto stepify(const Spec<T>& spec, T v) -> T {
	return spec.stepify ? spec.stepify(v) : v;
}

template <class T>
auto constrain(const Spec<T>& spec, T v) -> T {
	return spec.constrain ? spec.constrain(v) : v;
}

auto snap_value(const Spec<float>& spec, float v, float step_size, float snap_amount) -> float {
	return stepify(spec, snap_value(v, step_size, snap_amount));
}

template <class T>
auto increment(const Spec<T>& spec, T v, bool precise) -> T {
	return constrain(spec, stepify(spec, raw_increment(spec, v, precise)));
}

template <class T>
auto decrement(const Spec<T>& spec, T v, bool precise) -> T {
	return constrain(spec, stepify(spec, raw_decrement(spec, v, precise)));
}

template <class T>
auto drag(const Spec<T>& spec, T v, int amount, bool precise) -> T {
	return constrain(spec, stepify(spec, raw_drag(spec, v, amount, precise)));
}

template <class T>
auto to_string(const Spec<T>& spec, T v) -> std::string {
	return spec.to_string ? spec.to_string(v) : "";
}

template <class T>
auto from_string(const ::std::string& str) -> T {
	return spec.from_string ? spec.from_string(str) : T(0);
}

template <class T>
auto raw_increment(const Spec<T>& spec, T v, bool precise) -> T {
	return spec.increment ? spec.increment(v, precise) : v;
}

template <class T>
auto raw_decrement(const Spec<T>& spec, T v, bool precise) -> T {
	return spec.decrement ? spec.decrement(v, precise) : v;
}

template <class T>
auto raw_drag(const Spec<T>& spec, T v, int amount, bool precise) -> T {
	return spec.drag ? spec.drag(v, amount, precise) : v;
}

} // tweak
