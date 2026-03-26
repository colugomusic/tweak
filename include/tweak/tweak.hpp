#pragma once

#include <functional>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include "math.hpp"

namespace tweak {

template <class T> [[nodiscard]] auto find_number(const std::string& str) -> std::optional<T>;
template <class T> [[nodiscard]] auto find_positive_number(const std::string& str) -> std::optional<T>;

template <> [[nodiscard]]
inline auto find_number<float>(const std::string& str) -> std::optional<float> {
	auto r     = std::regex{"(\\-?\\s*[\\.\\d]+)"};
	auto match = std::smatch{};
	if (!std::regex_search(str, match, r)) { return std::nullopt; }
	try                                    { return std::stof(match[0].str()); }
	catch (const std::exception&)          { return std::nullopt; }
}

template <> [[nodiscard]]
inline auto find_number<int>(const std::string& str) -> std::optional<int> {
	auto r     = std::regex{"(\\-?\\s*[\\d]+)"};
	auto match = std::smatch{};
	if (!std::regex_search(str, match, r)) { return std::nullopt; }
	try                                    { return std::stoi(match[0].str()); }
	catch (const std::exception&)          { return std::nullopt; }
}

template <> [[nodiscard]]
inline auto find_positive_number<float>(const std::string& str) -> std::optional<float> {
	auto r     = std::regex{"([\\.\\d]+)"};
	auto match = std::smatch{};
	if (!std::regex_search(str, match, r)) { return std::nullopt; }
	try                                    { return std::stof(match[0].str()); }
	catch (const std::exception&)          { return std::nullopt; }
}

template <> [[nodiscard]]
inline auto find_positive_number<int>(const std::string& str) -> std::optional<int> {
	auto r     = std::regex{"([\\d]+)"};
	auto match = std::smatch{};
	if (!std::regex_search(str, match, r)) { return std::nullopt; }
	try                                    { return std::stoi(match[0].str()); }
	catch (const std::exception&)          { return std::nullopt; }
}

template <int Normal, int Precise, std::floating_point T> [[nodiscard]] constexpr
auto increment(T v, bool precise) -> T {
	return v + T(1) / (precise ? Precise : Normal);
}

template <int Normal, int Precise, std::floating_point T> [[nodiscard]] constexpr
auto decrement(T v, bool precise) -> T {
	return v - T(1) / (precise ? Precise : Normal);
}

template <int Normal, int Precise, std::integral T> [[nodiscard]] constexpr
auto increment(T v, bool precise) -> T {
	return v + (precise ? Precise : Normal);
}

template <int Normal, int Precise, std::integral T> [[nodiscard]] constexpr
auto decrement(T v, bool precise) -> T {
	return v - (precise ? Precise : Normal);
}

template <int Normal, std::floating_point T> [[nodiscard]] constexpr
auto increment(T v) -> T {
	return v + T(1) / (Normal);
}

template <int Normal, std::floating_point T> [[nodiscard]] constexpr
auto decrement(T v) -> T {
	return v - T(1) / (Normal);
}

template <int Normal, std::integral T> [[nodiscard]] constexpr
auto increment(T v) -> T {
	return v + Normal;
}

template <int Normal, std::integral T> [[nodiscard]] constexpr
auto decrement(T v) -> T {
	return v - Normal;
}

template <class T, int Normal, int Precise> [[nodiscard]] constexpr
auto drag(T v, int amount, bool precise) -> T {
	return v + T(static_cast<float>(amount) / (precise ? Precise : Normal));
}

template <class T, int Normal> [[nodiscard]] constexpr
auto drag(T v, int amount) -> T {
	return v + T(static_cast<float>(amount) / Normal);
}

template <class T> [[nodiscard]] constexpr
auto constrain(T v, T min, T max) -> T {
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

template <class T> [[nodiscard]]
auto to_string(T v) -> std::string {
	std::stringstream ss;
	ss << v;
	return ss.str();
}

template <std::floating_point T> [[nodiscard]]
auto snap_value(T v, T step_size, T snap_amount) -> T {
	if (step_size == T(0))   { return v; }
	if (snap_amount <= T(0)) { return v; }
	if (snap_amount >= T(1)) {
		v /= step_size;
		v = std::round(v);
		v *= step_size;
		return v;
	}
	const auto up   = std::ceil((v / step_size) + T(0.0001)) * step_size;
	const auto down = std::floor(v / step_size) * step_size;
	const auto x    = math::inverse_lerp(down, up, v);
	const auto t    = x * T(2);
	const auto i    = T(1) + (std::pow(snap_amount, T(4)) * T(99));
	const auto curve =
		t < T(1)
		? T(1) - (T(0.5) * (std::pow(T(1) - t, T(1) / i) + T(1)))
		: T(0.5) * (std::pow(t - T(1), T(1) / i) + T(1));
	return math::lerp(down, up, curve);
}

} // tweak
