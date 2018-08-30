#include "test_shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using BitArray = hfsm2::detail::BitArrayT<16>;

//------------------------------------------------------------------------------

void
verifySingleEntry(const BitArray& bitArray,
				  const hfsm2::ShortIndex index)
{
	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i)
		if (i == index)
			REQUIRE( bitArray[i]);
		else
			REQUIRE(!bitArray[i]);
}

//------------------------------------------------------------------------------

TEST_CASE("BitArray<> test", "[lib]") {
	BitArray bitArray;
	const auto capacity = bitArray.capacity;

	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i)
		REQUIRE(!bitArray[i]);

	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i) {
		bitArray[i] = true;
		verifySingleEntry(bitArray, i);

		bitArray.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////
