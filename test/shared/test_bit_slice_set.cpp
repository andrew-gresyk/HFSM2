// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "../tools.hpp"

#include <type_traits> // std::is_empty<>

namespace test_bit_slice_set {

////////////////////////////////////////////////////////////////////////////////

template <typename TBitSet>
struct BitSliceSetTests {
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void defaultConstruction() {
		TBitSet bits;

		CHECK(bits.empty());
		CHECK(!static_cast<bool>(bits.template cbits<0, TBitSet::CAPACITY>()));
	}

	static void clearAll() {
		TBitSet bits;
		auto whole = bits.template bits<0, TBitSet::CAPACITY>();

		whole.set(1u);
		whole.set(9u);
		CHECK(!bits.empty());

		bits.clear();

		CHECK(bits.empty());
		CHECK(!static_cast<bool>(bits.template cbits<0, TBitSet::CAPACITY>()));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void compileTimeViews() {
		TBitSet bits;

		auto view = bits.template bits<1, 10>();
		CHECK(!static_cast<bool>(view));

		view.set(0u);
		view.set(7u);
		view.set(9u);

		const TBitSet& cbits = bits;
		auto cview = cbits.template cbits<1, 10>();
		CHECK(static_cast<bool>(cview));
		CHECK( cview.get(0u));
		CHECK(!cview.get(1u));
		CHECK( cview.get(7u));
		CHECK(!cview.get(8u));
		CHECK( cview.get(9u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void byteAlignedView() {
		TBitSet bits;
		auto view = bits.template bits<1, 8>();

		CHECK(!static_cast<bool>(view));

		auto whole = bits.template bits<0, TBitSet::CAPACITY>();
		whole.set(7u);
		whole.set(8u);
		whole.set(15u);
		whole.set(16u);

		CHECK(static_cast<bool>(view));

		const TBitSet& cbits = bits;
		auto cview = cbits.template cbits<1, 8>();
		CHECK( cview.get(0u));
		CHECK(!cview.get(1u));
		CHECK( cview.get(7u));

		view.clear();

		auto wholeView = cbits.template cbits<0, TBitSet::CAPACITY>();
		CHECK(!static_cast<bool>(view));
		CHECK( wholeView.get(7u));
		CHECK(!wholeView.get(8u));
		CHECK(!wholeView.get(15u));
		CHECK( wholeView.get(16u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void runtimeViewOffsetSpan() {
		TBitSet bits;
		auto whole = bits.template bits<0, TBitSet::CAPACITY>();

		whole.set(5u);
		whole.set(18u);

		hfsm2::detail::BitSlice units{1, 10};
		auto view = bits.bits(units);

		CHECK(!static_cast<bool>(view));

		view.set(0u);
		view.set(7u);
		view.set(9u);

		const TBitSet& cbits = bits;
		auto wholeView = cbits.template cbits<0, TBitSet::CAPACITY>();
		auto sliceView = cbits.template cbits<1, 10>();
		auto runtimeSliceView = cbits.cbits(units);

		CHECK( wholeView.get(5u));
		CHECK( wholeView.get(8u));
		CHECK( wholeView.get(15u));
		CHECK( wholeView.get(17u));
		CHECK( wholeView.get(18u));
		CHECK(!wholeView.get(7u));
		CHECK(!wholeView.get(16u));

		CHECK(static_cast<bool>(sliceView));
		CHECK( sliceView.get(0u));
		CHECK(!sliceView.get(1u));
		CHECK( sliceView.get(7u));
		CHECK(!sliceView.get(8u));
		CHECK( sliceView.get(9u));
		CHECK(static_cast<bool>(runtimeSliceView));
		CHECK( runtimeSliceView.get(0u));
		CHECK(!runtimeSliceView.get(1u));
		CHECK( runtimeSliceView.get(7u));
		CHECK(!runtimeSliceView.get(8u));
		CHECK( runtimeSliceView.get(9u));

		view.clear();

		auto clearedView = cbits.template cbits<0, TBitSet::CAPACITY>();
		CHECK(!static_cast<bool>(bits.template cbits<1, 10>()));
		CHECK( clearedView.get(5u));
		CHECK(!clearedView.get(8u));
		CHECK(!clearedView.get(15u));
		CHECK(!clearedView.get(17u));
		CHECK( clearedView.get(18u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void viewClearPreservesAdjacentBits() {
		TBitSet bits;
		auto whole = bits.template bits<0, TBitSet::CAPACITY>();

		whole.set(5u);
		whole.set(6u);
		whole.set(7u);
		whole.set(10u);

		auto view = bits.template bits<0, 5>();
		view.set(1u);
		view.set(4u);
		view.clear();

		const TBitSet& cbits = bits;
		auto wholeView = cbits.template cbits<0, TBitSet::CAPACITY>();

		CHECK(!static_cast<bool>(view));
		CHECK(!wholeView.get(1u));
		CHECK(!wholeView.get(4u));
		CHECK( wholeView.get(5u));
		CHECK( wholeView.get(6u));
		CHECK( wholeView.get(7u));
		CHECK( wholeView.get(10u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void inequality() {
		TBitSet left;
		TBitSet right;

		CHECK(!(left != right));

		left.template bits<1, 10>().set(3u);

		CHECK(left != right);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

using BSS24 = hfsm2::detail::BitSliceSetT<24>;

//------------------------------------------------------------------------------

TEST_CASE("Shared.BitSliceSetT<24>.DefaultConstruction") {
	BitSliceSetTests<BSS24>::defaultConstruction();
}

TEST_CASE("Shared.BitSliceSetT<24>.ClearAll") {
	BitSliceSetTests<BSS24>::clearAll();
}

TEST_CASE("Shared.BitSliceSetT<24>.CompileTimeViews") {
	BitSliceSetTests<BSS24>::compileTimeViews();
}

TEST_CASE("Shared.BitSliceSetT<24>.ByteAlignedView") {
	BitSliceSetTests<BSS24>::byteAlignedView();
}

TEST_CASE("Shared.BitSliceSetT<24>.RuntimeViewOffsetSpan") {
	BitSliceSetTests<BSS24>::runtimeViewOffsetSpan();
}

TEST_CASE("Shared.BitSliceSetT<24>.ViewClearPreservesAdjacentBits") {
	BitSliceSetTests<BSS24>::viewClearPreservesAdjacentBits();
}

TEST_CASE("Shared.BitSliceSetT<24>.Inequality") {
	BitSliceSetTests<BSS24>::inequality();
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.BitSliceSetT<0>.ZeroCapacity") {
	using EmptySlice = hfsm2::detail::BitSliceSetT<0>;

	static_assert(std::is_empty<EmptySlice>::value,
		"BitSliceSetT<0> should be an empty type");

	EmptySlice bits;
	bits.clear();
	CHECK(bits.empty());
}

////////////////////////////////////////////////////////////////////////////////

}
