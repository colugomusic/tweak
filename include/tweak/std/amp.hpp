#pragma once

#include <optional>
#include <sstream>
#include <string>
#include "../convert.hpp"
#include "../tweak.hpp"

namespace tweak {
namespace std {
namespace amp {

static constexpr auto SILENT { 0.0f };

inline auto stepify(float v) -> float
{
	static constexpr auto MIN_THRESHOLD { 0.00001f };

	if (v <= MIN_THRESHOLD) return SILENT;

	return convert::db_to_linear(math::stepify(convert::linear_to_db(v), 0.1f));
}

inline auto constrain(float v)
{
	const auto db = convert::linear_to_db(v);

	if (db < -60.0f) return SILENT;
	if (db > 12.0f) return convert::db_to_linear(12.0f);

	return v;
}

inline auto from_string(const ::std::string& str) -> ::std::optional<float>
{
	auto db = tweak::find_number<float>(str);

	if (!db) return db;

	return convert::db_to_linear(*db);
};

inline auto db_to_string(float db)
{
	::std::stringstream ss;
	
	ss << db << " dB";

	return ss.str();
}

inline auto to_string(float v)
{
	::std::stringstream ss;

	if (v <= SILENT)
	{
		ss << "Silent";
	}
	else
	{
		ss << db_to_string(math::stepify(convert::linear_to_db(v), 0.1f));
	}

	return ss.str();
}

inline auto increment(float v, bool precise)
{
	if (v <= SILENT) return convert::db_to_linear(-60.0f);

	return convert::db_to_linear(tweak::increment<1, 10>(convert::linear_to_db(v), precise));
};

inline auto decrement(float v, bool precise)
{
	return convert::db_to_linear(tweak::decrement<1, 10>(convert::linear_to_db(v), precise));
};

inline auto drag(float v, int amount, bool precise) -> float
{
	if (v <= SILENT) v = convert::db_to_linear(-61.0f);

	return convert::db_to_linear(tweak::drag<float, 1, 10>(convert::linear_to_db(v), amount / 5, precise));
};

} // amp
} // std
} // tweak