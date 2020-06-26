#define HFSM2_ENABLE_UTILITY_THEORY
#include "../tools.hpp"

namespace test_random {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
void
testUniformity(const int average) {
	using Type = T;
	using Random = hfsm2::RandomT<Type>;

	Random random{0};
	int histogram[10] = {0};

	for (int i = 0; i < 10 * average; ++i) {
		const float real = random.next();
		REQUIRE(real >= Type{0.0});
		REQUIRE(real  < Type{1.0});

		const unsigned n = (unsigned) (10.0f * real);
		REQUIRE(n < hfsm2::detail::count(histogram));

		++histogram[n];
	}

	for (unsigned i = 1; i < hfsm2::detail::count(histogram); ++i) {
		const int delta = abs(average - histogram[i]);
		const float relative = 1.0f * delta / average;

		REQUIRE(relative < 0.2f);
	}
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.RandomT<>", "[shared]") {
	testUniformity<float>(100);
}

////////////////////////////////////////////////////////////////////////////////

}
