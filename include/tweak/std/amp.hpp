#pragma once

#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak::std_::amp {

static constexpr auto SILENT = 0.0f;

template <std::floating_point T> [[nodiscard]] constexpr
auto stepify(T v) -> T {
	static constexpr auto MIN_THRESHOLD = T(0.00001);
	if (v <= MIN_THRESHOLD) { return SILENT; }
	else                    { return convert::db_to_linear(math::stepify(convert::linear_to_db(v), T(0.1))); }
}

template <std::floating_point T> [[nodiscard]] constexpr
auto constrain(T v) -> T {
	const auto db = convert::linear_to_db(v);
	     if (db < -60.0f) { return SILENT; }
	else if (db > 12.0f)  { return convert::db_to_linear(12.0f); }
	else                  { return v; }
}

template <std::floating_point T = float> [[nodiscard]]
auto from_string(const std::string& str) -> std::optional<T> {
	auto db = tweak::find_number<float>(str);
	if (!db) { return db; }
	else     { return convert::db_to_linear(*db); }
}

template <std::floating_point T> [[nodiscard]]
auto db_to_string(T db) -> std::string {
	auto ss = std::stringstream{};
	ss << db << " dB";
	return ss.str();
}

template <std::floating_point T> [[nodiscard]]
auto to_string(T v) -> std::string {
	auto ss = std::stringstream{};
	if (v <= SILENT) { ss << "Silent"; }
	else             { ss << db_to_string(math::stepify(convert::linear_to_db(v), T(0.1))); }
	return ss.str();
}

template <std::floating_point T> [[nodiscard]] constexpr
auto increment(T v, bool precise) -> T {
	if (v <= SILENT) { return convert::db_to_linear(T(-60)); }
	else             { return convert::db_to_linear(tweak::increment<1, 10>(convert::linear_to_db(v), precise)); }
};

template <std::floating_point T> [[nodiscard]] constexpr
auto decrement(T v, bool precise) -> T {
	if (v <= SILENT) { return v; }
	else             { return convert::db_to_linear(tweak::decrement<1, 10>(convert::linear_to_db(v), precise)); }
};

template <std::floating_point T> [[nodiscard]] constexpr
auto drag(T v, int amount, bool precise) -> T {
	if (v <= SILENT) { v = convert::db_to_linear(T(-61)); }
	return convert::db_to_linear(tweak::drag<float, 1, 10>(convert::linear_to_db(v), amount / 5, precise));
};

} // tweak::std_::amp
