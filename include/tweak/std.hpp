#pragma once

#include <optional>
#include <sstream>
#include <string>
#include "convert.hpp"

namespace tweak {
namespace std {

namespace amp {

inline auto stepify(float v) -> float
{
	if (v <= 0.00001f) return 0.0f;

	return convert::db_to_linear(math::stepify(convert::linear_to_db(v), 0.1f));
}

inline auto constrain(float v)
{
	const auto db = convert::linear_to_db(v);

	if (db < -60.0f) return 0.0f;
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

	if (v <= 0.0f)
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
	if (v <= 0.0f) return convert::db_to_linear(-60.0f);

	return convert::db_to_linear(tweak::increment<1, 10>(convert::linear_to_db(v), precise));
};

inline auto decrement(float v, bool precise)
{
	return convert::db_to_linear(tweak::decrement<1, 10>(convert::linear_to_db(v), precise));
};

inline auto drag(float v, int amount, bool precise) -> float
{
	if (v <= 0.0f) v = convert::db_to_linear(-61.0f);

	return convert::db_to_linear(tweak::drag<float, 1, 10>(convert::linear_to_db(v), amount / 5, precise));
};

} // amp

namespace percentage {

inline auto stepify(float v)
{
	return tweak::math::stepify<1000>(v);
}

inline auto constrain(float v)
{
	return ::std::clamp(v, 0.0f, 1.0f);
};

inline auto increment(float v, bool precise)
{
	return tweak::increment<100, 1000>(v, precise);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<100, 1000>(v, precise);
};

inline auto drag(float v, int amount, bool precise)
{
	return tweak::drag<float, 100, 1000>(v, amount / 5, precise);
};

inline auto to_string(float v)
{
	::std::stringstream ss;

	ss << stepify(v * 100.0f) << "%";

	return ss.str();
}

inline auto from_string(const ::std::string& str) -> ::std::optional<float>
{
	auto value = tweak::find_number<float>(str);

	if (!value) return ::std::optional<float>();

	return (*value / 100.0f);
};

} // percentage

namespace percentage_bipolar {

inline auto stepify(float v)
{
	return math::stepify(v, 0.005f);
}

inline auto to_string(float v)
{
	::std::stringstream ss;

	ss << percentage::stepify((v - 0.5f) * 200.0f) << "%";

	return ss.str();
}

inline auto increment(float v, bool precise)
{
	return tweak::increment<200, 2000>(v, precise);
};

inline auto decrement(float v, bool precise)
{
	return tweak::decrement<200, 2000>(v, precise);
};

inline auto drag(float v, int amount, bool precise)
{
	return tweak::drag<float, 200, 2000>(v, amount / 5, precise);
};

inline auto from_string(const ::std::string& str)
{
	return percentage::from_string(str);
};

} // percentage_bipolar

} // std
} // tweak