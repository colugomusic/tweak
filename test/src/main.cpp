#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <tweak/convert.hpp>
#include <tweak/math.hpp>
#include <tweak/tweak.hpp>

TEST_CASE("non-finite conversion values") {
	REQUIRE(tweak::convert::db_to_linear(std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity());
	REQUIRE(tweak::convert::linear_to_db(std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity());
	REQUIRE(std::isnan(tweak::convert::db_to_linear(std::numeric_limits<float>::quiet_NaN())));
	REQUIRE(std::isnan(tweak::convert::linear_to_db(std::numeric_limits<float>::quiet_NaN())));
}
