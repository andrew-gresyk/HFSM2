#include "../shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using BitArray = hfsm2::detail::BitArrayStorageT<hfsm2::ShortIndex, 16>;

//------------------------------------------------------------------------------

void
verifySingleEntry(const BitArray& bitArray,
				  const hfsm2::ShortIndex index)
{
	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i)
		if (i == index)
			REQUIRE( bitArray[i]); //-V521
		else
			REQUIRE(!bitArray[i]); //-V521
}

//------------------------------------------------------------------------------

TEST_CASE("BitArrayT<> test", "[shared]") {
	BitArray bitArray;

	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i)
		REQUIRE(!bitArray[i]); //-V521

	for (hfsm2::ShortIndex i = 0; i < bitArray.capacity; ++i) {
		bitArray[i] = true; //-V601
		verifySingleEntry(bitArray, i);

		bitArray.clear();
	}
}

////////////////////////////////////////////////////////////////////////////////
