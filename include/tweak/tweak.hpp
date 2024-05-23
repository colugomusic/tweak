#pragma once

#include <functional>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include "math.hpp"

namespace tweak {

template <class T> auto find_number(std::string str) -> std::optional<T>;
template <class T> auto find_positive_number(std::string str) -> std::optional<T>;

template <> inline
auto find_number<float>(std::string str) -> std::optional<float> {
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

template <> inline
auto find_number<int>(std::string str) -> std::optional<int> {
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

template <> inline
auto find_positive_number<float>(std::string str) -> std::optional<float> {
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

template <> [[nodiscard]] inline
auto find_positive_number<int>(std::string str) -> std::optional<int> {
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

template <int Normal, int Precise> [[nodiscard]]
auto increment(float v, bool precise) -> float {
	return v + 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]]
auto decrement(float v, bool precise) -> float {
	return v - 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]]
auto increment(int v, bool precise) -> int {
	return v + (precise ? Precise : Normal);
}

template <int Normal, int Precise> [[nodiscard]]
auto decrement(int v, bool precise) -> int {
	return v - (precise ? Precise : Normal);
}

template <int Normal> [[nodiscard]]
auto increment(float v) -> float {
	return v + 1.0f / (Normal);
}

template <int Normal> [[nodiscard]]
auto decrement(float v) -> float {
	return v - 1.0f / (Normal);
}

template <int Normal> [[nodiscard]]
auto increment(int v) -> int {
	return v + Normal;
}

template <int Normal> [[nodiscard]]
auto decrement(int v) -> int {
	return v - Normal;
}

template <class T, int Normal, int Precise> [[nodiscard]]
auto drag(T v, int amount, bool precise) -> T {
	return v + T(float(amount) / (precise ? Precise : Normal));
}

template <class T, int Normal> [[nodiscard]]
auto drag(T v, int amount) -> T {
	return v + T(float(amount) / Normal);
}

template <class T> [[nodiscard]]
auto constrain(T v, T min, T max) -> T {
	if (v < min) return min;
	if (v > max) return max; 
	return v;
}

template <class T> [[nodiscard]]
auto to_string(T v) -> ::std::string {
	::std::stringstream ss; 
	ss << v; 
	return ss.str();
}

[[nodiscard]] inline
auto snap_value(float v, float step_size, float snap_amount) -> float {
	if (step_size == 0.0f) return v;
	if (snap_amount <= 0.0f) return v; 
	if (snap_amount >= 1.0f) {
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
template <class T> using FromStringFunc = auto (*)(std::string_view) -> std::optional<T>;
template <class T> using ConstrainFunc  = auto (*)(T) -> T;
template <class T> using DragFunc       = auto (*)(T, int, bool) -> T;
template <class T> using IncrementFunc  = auto (*)(T, bool) -> T;
template <class T> using DecrementFunc  = auto (*)(T, bool) -> T;
template <class T> using StepifyFunc    = auto (*)(T) -> T;

template <class T>
struct Spec {
	ToStringFunc<T> to_string     = nullptr;
	FromStringFunc<T> from_string = nullptr;
	ConstrainFunc<T> constrain    = nullptr;
	DragFunc<T> drag              = nullptr;
	IncrementFunc<T> increment    = nullptr;
	DecrementFunc<T> decrement    = nullptr;
	StepifyFunc<T> stepify        = nullptr;
};

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
auto from_string(const Spec<T>& spec, ::std::string_view str) -> std::optional<T> {
	return spec.from_string ? spec.from_string(str) : std::nullopt;
}

} // tweak
