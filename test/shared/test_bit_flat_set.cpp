// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "../tools.hpp"

#include <type_traits> // std::is_empty<>

namespace test_flat_bit_set {

////////////////////////////////////////////////////////////////////////////////

template <typename TBitSet>
struct BitFlatSetTests {
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void defaultConstruction() {
		TBitSet bits;

		CHECK(bits.empty());

		for (unsigned i = 0; i < TBitSet::CAPACITY; ++i)
			CHECK(!bits.get(i));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void setAll() {
		TBitSet bits;

		bits.set();
		CHECK(!bits.empty());

		for (unsigned i = 0; i < TBitSet::CAPACITY; ++i)
			CHECK(bits.get(i));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void clearAll() {
		TBitSet bits;

		bits.set();
		bits.clear();

		CHECK(bits.empty());

		for (unsigned i = 0; i < TBitSet::CAPACITY; ++i)
			CHECK(!bits.get(i));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void runtimeGetSetClear() {
		TBitSet bits;

		bits.set(0u);
		CHECK( bits.get(0u));
		CHECK(!bits.get(1u));

		bits.set(7u);
		CHECK(bits.get(7u));

		bits.clear(0u);
		CHECK(!bits.get(0u));
		CHECK( bits.get(7u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void runtimeCrossByteGetSetClear() {
		static_assert(TBitSet::CAPACITY > 8, "");

		TBitSet bits;
		const unsigned last = TBitSet::CAPACITY - 1;

		bits.set(8u);
		bits.set(last);

		CHECK(!bits.get(7u));
		CHECK( bits.get(8u));
		CHECK( bits.get(last));

		bits.clear(8u);
		CHECK(!bits.get(8u));
		CHECK( bits.get(last));

		bits.clear(last);
		CHECK(bits.empty());
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void templateGet() {
		TBitSet bits;

		bits.set(0u);
		bits.set(5u);

		CHECK( bits.template get<0>());
		CHECK(!bits.template get<1>());
		CHECK( bits.template get<5>());

		bits.clear(0u);
		CHECK(!bits.template get<0>());
		CHECK( bits.template get<5>());
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void operatorAndAssign() {
		TBitSet a;
		TBitSet b;

		a.set();
		b.set(2u);
		b.set(4u);

		a &= b;

		CHECK(!a.empty());
		CHECK(!a.get(1u));
		CHECK( a.get(2u));
		CHECK( a.get(4u));
		CHECK(!a.get(5u));
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	static void trailingBitsMasked() {
		TBitSet bits;

		bits.set();

		for (unsigned i = 0; i < TBitSet::CAPACITY; ++i)
			CHECK(bits.get(i));

		CHECK(!bits.empty());
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

using FBS13 = hfsm2::detail::BitFlatSetT<13>;
using FBS16 = hfsm2::detail::BitFlatSetT<16>;

//------------------------------------------------------------------------------

TEST_CASE("Shared.BitFlatSetT<13>.DefaultConstruction") {
	BitFlatSetTests<FBS13>::defaultConstruction();
}

TEST_CASE("Shared.BitFlatSetT<13>.SetAll") {
	BitFlatSetTests<FBS13>::setAll();
}

TEST_CASE("Shared.BitFlatSetT<13>.ClearAll") {
	BitFlatSetTests<FBS13>::clearAll();
}

TEST_CASE("Shared.BitFlatSetT<13>.RuntimeGetSetClear") {
	BitFlatSetTests<FBS13>::runtimeGetSetClear();
}

TEST_CASE("Shared.BitFlatSetT<13>.RuntimeCrossByteGetSetClear") {
	BitFlatSetTests<FBS13>::runtimeCrossByteGetSetClear();
}

TEST_CASE("Shared.BitFlatSetT<13>.TemplateGet") {
	BitFlatSetTests<FBS13>::templateGet();
}

TEST_CASE("Shared.BitFlatSetT<13>.OperatorAndAssign") {
	BitFlatSetTests<FBS13>::operatorAndAssign();
}

TEST_CASE("Shared.BitFlatSetT<13>.TrailingBitsMasked") {
	BitFlatSetTests<FBS13>::trailingBitsMasked();
}

TEST_CASE("Shared.BitFlatSetT<16>.SetAll") {
	BitFlatSetTests<FBS16>::setAll();
}

//------------------------------------------------------------------------------

TEST_CASE("Shared.BitFlatSetT<0>.ZeroCapacity") {
	using EmptyFlat = hfsm2::detail::BitFlatSetT<0>;

	static_assert(std::is_empty<EmptyFlat>::value,
		"BitFlatSetT<0> should be an empty type");

	EmptyFlat bits;
	bits.clear();
	CHECK(bits.empty());
}

////////////////////////////////////////////////////////////////////////////////

}
