// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "../tools.hpp"

#include <type_traits>

namespace test_dynamic_array {

////////////////////////////////////////////////////////////////////////////////

static unsigned s_constructed       = 0;
static unsigned s_destructed        = 0;
static unsigned s_copied            = 0;
static unsigned s_moved             = 0;
static unsigned s_destroyedOrder[8] = {};
static unsigned s_destroyedCount    = 0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Tracked final {
	unsigned value;

	explicit Tracked(const unsigned value_) noexcept
		: value{value_}
	{
		++s_constructed;
	}

	Tracked(const Tracked& other) noexcept
		: value{other.value}
	{
		++s_constructed;
		++s_copied;
	}

	Tracked(Tracked&& other) noexcept
		: value{other.value}
	{
		other.value = 0;
		++s_constructed;
		++s_moved;
	}

	~Tracked() noexcept {
		++s_destructed;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct alignas(32) OverAlignedTracked final {
	unsigned values[8];

	explicit OverAlignedTracked(const unsigned value) noexcept
		: values{value, 0, 0, 0, 0, 0, 0, 0}
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct OrderedTracked final {
	unsigned value;

	explicit OrderedTracked(const unsigned value_) noexcept
		: value{value_}
	{}

	~OrderedTracked() noexcept {
		s_destroyedOrder[s_destroyedCount++] = value;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using TrackedArray		= hfsm2::detail::DynamicArrayT<Tracked, 8>;
using SmallTrackedArray = hfsm2::detail::DynamicArrayT<Tracked, 4>;
using OverAlignedArray	= hfsm2::detail::DynamicArrayT<OverAlignedTracked, 4>;
using OrderedArray		= hfsm2::detail::DynamicArrayT<OrderedTracked, 4>;
using EmptyArray		= hfsm2::detail::DynamicArrayT<int, 0>;

static_assert(std::is_empty<EmptyArray>::value,
			  "capacity-0 dynamic array should carry no element storage");

//------------------------------------------------------------------------------

static void resetCounters() noexcept {
	s_constructed    = 0;
	s_destructed     = 0;
	s_copied         = 0;
	s_moved          = 0;
	s_destroyedCount = 0;

	for (unsigned& value : s_destroyedOrder)
		value = 0;
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<Tracked,8>.ClearDestroysAndReusesStorage") {
	resetCounters();

	TrackedArray array;
	array.emplace(1u);
	array.emplace(2u);
	array.emplace(3u);

	CHECK(array.count() == 3);
	CHECK(s_constructed == 3);
	CHECK(s_destructed  == 0);

	array.clear();

	CHECK(array.empty());
	CHECK(array.count() == 0);
	CHECK(s_destructed  == 3);

	const unsigned constructedBefore = s_constructed;

	const auto index = array.emplace(77u);
	CHECK(index == 0);
	CHECK(array.count() == 1);
	CHECK(array[0].value == 77u);
	CHECK(s_constructed - constructedBefore == 1);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<Tracked,8>.CopyMoveAndSelfAssignment") {
	resetCounters();

	TrackedArray original;
	original.emplace(10u);
	original.emplace(20u);

	const unsigned constructedBeforeCopy = s_constructed;
	const unsigned copiedBeforeCopy      = s_copied;

	TrackedArray copy{original};

	CHECK(copy.count() == 2);
	CHECK(copy[0].value == 10u);
	CHECK(copy[1].value == 20u);
	CHECK(s_constructed - constructedBeforeCopy == 2);
	CHECK(s_copied      - copiedBeforeCopy      == 2);

	const unsigned destructedBeforeSelfCopy = s_destructed;

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wself-assign-overloaded" // error: explicitly assigning value of variable of type '' to itself
#endif

	copy = copy;

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	CHECK(copy.count()  == 2);
	CHECK(copy[0].value == 10u);
	CHECK(copy[1].value == 20u);
	CHECK(s_destructed - destructedBeforeSelfCopy == 0);

	const unsigned destructedBeforeSelfMove = s_destructed;

	copy = hfsm2::move(copy);

	CHECK(copy.count()  == 2);
	CHECK(copy[0].value == 10u);
	CHECK(copy[1].value == 20u);
	CHECK(s_destructed - destructedBeforeSelfMove == 0);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<Tracked,8>.AssignmentAndAppend") {
	resetCounters();

	TrackedArray source;
	source.emplace(10u);

	TrackedArray target;
	target.emplace(99u);

	const unsigned destructedBeforeCopyAssign = s_destructed;
	const unsigned copiedBeforeCopyAssign     = s_copied;

	target = source;

	CHECK(s_destructed - destructedBeforeCopyAssign == 1);
	CHECK(s_copied     - copiedBeforeCopyAssign     == 1);
	CHECK(target.count()  == 1);
	CHECK(target[0].value == 10u);

	TrackedArray moved{hfsm2::move(source)};
	CHECK(moved.count()  == 1);
	CHECK(moved[0].value == 10u);
	CHECK(source.empty());

	TrackedArray other;
	other.emplace(20u);
	other.emplace(30u);

	const unsigned movedBeforeAssign      = s_moved;
	const unsigned destructedBeforeAssign = s_destructed;

	moved = hfsm2::move(other);

	CHECK(moved.count()  == 2);
	CHECK(moved[0].value == 20u);
	CHECK(moved[1].value == 30u);
	CHECK(other.empty());
	CHECK(s_moved      - movedBeforeAssign      == 2);
	CHECK(s_destructed - destructedBeforeAssign == 3);

	SmallTrackedArray a;
	a.emplace(1u);
	a.emplace(2u);

	SmallTrackedArray b;
	b.emplace(3u);
	b.emplace(4u);

	const unsigned copiedBeforeAppend = s_copied;

	a += b;

	CHECK(a.count()  == 4);
	CHECK(a[0].value == 1u);
	CHECK(a[1].value == 2u);
	CHECK(a[2].value == 3u);
	CHECK(a[3].value == 4u);
	CHECK(s_copied - copiedBeforeAppend == 2);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<Tracked,8>.IteratorComparisonUsesCursor") {
	TrackedArray array;
	array.emplace(10u);
	array.emplace(20u);

	auto begin = array.begin();
	auto same  = array.begin();
	auto end   = array.end();

	CHECK(!(begin != same));
	CHECK(begin != end);

	++same;
	CHECK(begin != same);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<OrderedTracked,4>.ClearDestroysInReverseOrder") {
	resetCounters();

	{
		OrderedArray array;
		array.emplace(1u);
		array.emplace(2u);
		array.emplace(3u);

		array.clear();
		CHECK(array.empty());
	}

	CHECK(s_destroyedCount == 3);
	CHECK(s_destroyedOrder[0] == 3u);
	CHECK(s_destroyedOrder[1] == 2u);
	CHECK(s_destroyedOrder[2] == 1u);
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.DynamicArray<OverAlignedTracked,4>.SupportsOverAlignment") {
	OverAlignedArray array;

	const auto i0 = array.emplace(11u);
	const auto i1 = array.emplace(22u);

	CHECK(i0 == 0);
	CHECK(i1 == 1);
	CHECK(array[0].values[0] == 11u);
	CHECK(array[1].values[0] == 22u);
	CHECK(reinterpret_cast<uintptr_t>(&array[0]) % alignof(OverAlignedTracked) == 0);
	CHECK(reinterpret_cast<uintptr_t>(&array[1]) % alignof(OverAlignedTracked) == 0);
}

////////////////////////////////////////////////////////////////////////////////

}
