// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "../tools.hpp"

#include <type_traits>

namespace test_static_array {

////////////////////////////////////////////////////////////////////////////////

using Array4 = hfsm2::detail::StaticArrayT<int, 4>;
using Array0 = hfsm2::detail::StaticArrayT<int, 0>;

struct NonTrivial final {
	int value = 0;

	NonTrivial() noexcept = default;
	explicit NonTrivial(const int value_) noexcept
		: value{value_}
	{}

	NonTrivial(const NonTrivial& other) noexcept
		: value{other.value}
	{}

	NonTrivial& operator = (const NonTrivial& other) noexcept {
		value = other.value;

		return *this;
	}

	bool operator == (const NonTrivial& other) const noexcept	{ return value == other.value;	}
	bool operator != (const NonTrivial& other) const noexcept	{ return value != other.value;	}
};

static_assert(!std::is_trivial<NonTrivial>::value,
			  "NonTrivial should exercise non-trivial static array support");
static_assert(std::is_empty<Array0>::value,
			  "capacity-0 static array should carry no element storage");

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Shared.StaticArray<int,4>.DefaultAndFill") {
	Array4 array;

	CHECK(array.count() == 4);
	CHECK(array.empty());

	array.fill(7);
	CHECK(!array.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(array[i] == 7);

	array.clear();
	CHECK(array.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(array[i] == hfsm2::detail::filler<int>());
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<int,4>.Equality") {
	Array4 a;
	Array4 b;

	CHECK(a == b);
	CHECK(!(a != b));

	a[2] = 1;
	CHECK(a != b);
	CHECK(!(a == b));

	b[2] = 1;
	CHECK(a == b);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<int,0>.ZeroCapacity") {
	Array0 array;

	(void)array;
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<int,4>.FillerConstructor") {
	Array4 array(42);

	CHECK(!array.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(array[i] == 42);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<int,4>.Iteration") {
	Array4 array;
	array.fill(3);

	int sum = 0;
	for (const auto& value : array)
		sum += value;

	CHECK(sum == 12);

	hfsm2::Long count = 0;
	for (auto it = array.cbegin(); it != array.cend(); ++it) {
		CHECK(*it == 3);
		++count;
	}

	CHECK(count == 4);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<int,4>.IteratorComparisonUsesCursor") {
	Array4 array;
	array.fill(3);

	auto begin = array.begin();
	auto same  = array.begin();
	auto end   = array.end();

	CHECK(!(begin != same));
	CHECK(begin != end);

	++same;
	CHECK(begin != same);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<>.FillerShortAndLong") {
	hfsm2::detail::StaticArrayT<hfsm2::Short, 4> shorts;
	shorts.clear();
	CHECK(shorts.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(shorts[i] == hfsm2::detail::filler<hfsm2::Short>());

	hfsm2::detail::StaticArrayT<hfsm2::Long, 4> longs;
	longs.fill(0);
	longs.clear();
	CHECK(longs.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(longs[i] == hfsm2::detail::filler<hfsm2::Long>());
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.StaticArray<NonTrivial,4>.Support") {
	hfsm2::detail::StaticArrayT<NonTrivial, 4> items;

	CHECK(items.empty());

	items.fill(NonTrivial{42});
	CHECK(!items.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(items[i] == NonTrivial{42});

	items.clear();
	CHECK(items.empty());
	for (hfsm2::Long i = 0; i < 4; ++i)
		CHECK(items[i] == NonTrivial{});
}

////////////////////////////////////////////////////////////////////////////////

}
