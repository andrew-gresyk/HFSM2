#include "../shared.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Shared.RandomT<>", "[shared]") {
	using Random = hfsm2::RandomT<float>;

	Random random{0};
	int histogram[10] = {0};

	for (unsigned i = 0; i < 10000; ++i) {
		const float f = random.next();
		REQUIRE(f >= 0.0f);
		REQUIRE(f  < 1.0f);

		const unsigned n = (unsigned) (10.0f * f);
		REQUIRE(n < hfsm2::detail::count(histogram));

		++histogram[n];
	}

	int average = histogram[0];
	for (unsigned i = 1; i < hfsm2::detail::count(histogram); ++i)
		average += histogram[i];

	average /= hfsm2::detail::count(histogram);

	for (unsigned i = 1; i < hfsm2::detail::count(histogram); ++i) {
		const int absolute = abs(average - histogram[i]);
		const float relative = 1.0f * absolute / average;

		REQUIRE(relative < 0.1f);
	}
}

////////////////////////////////////////////////////////////////////////////////
