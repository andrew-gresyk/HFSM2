// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "../tools.hpp"

namespace test_bit_array {

////////////////////////////////////////////////////////////////////////////////

using BitArray = hfsm2::detail::BitArrayT<32>;
using Bits	   = typename BitArray::Bits;

TEST_CASE("Shared.BitArrayT<>") {
	BitArray bitArray;

	bitArray.clear();

	WHEN("Static methods") {
		{
			const Bits bits = bitArray.bits<3, 7>();
			REQUIRE(!bits);

			REQUIRE(!bits.get<0>());
			REQUIRE(!bits.get<1>());
			REQUIRE(!bits.get<2>());
			REQUIRE(!bits.get<3>());
			REQUIRE(!bits.get<4>());
			REQUIRE(!bits.get<5>());
			REQUIRE(!bits.get<6>());
		}

		{
			Bits bits = bitArray.bits<3, 7>();
			bits.set<4>();
		}

		{
			const Bits bits = bitArray.bits<2, 13>();
			REQUIRE(!bits.get< 0>());
			REQUIRE(!bits.get< 1>());
			REQUIRE(!bits.get< 2>());
			REQUIRE(!bits.get< 3>());
			REQUIRE(!bits.get< 4>());
			REQUIRE(!bits.get< 5>());
			REQUIRE(!bits.get< 6>());
			REQUIRE(!bits.get< 7>());
			REQUIRE(!bits.get< 8>());
			REQUIRE(!bits.get< 9>());
			REQUIRE(!bits.get<10>());
			REQUIRE(!bits.get<11>());
			REQUIRE( bits.get<12>());
		}

		{
			Bits bits = bitArray.bits<2, 13>();
			bits.clear<12>();
		}

		{
			const Bits bits = bitArray.bits<3, 7>();
			REQUIRE(!bits);

			REQUIRE(!bits.get<0>());
			REQUIRE(!bits.get<1>());
			REQUIRE(!bits.get<2>());
			REQUIRE(!bits.get<3>());
			REQUIRE(!bits.get<4>());
			REQUIRE(!bits.get<5>());
			REQUIRE(!bits.get<6>());
		}
	}

	WHEN("Dynamic methods") {
		{
			const Bits bits = bitArray.bits<3, 7>();
			REQUIRE(!bits);

			REQUIRE(!bits.get(0));
			REQUIRE(!bits.get(1));
			REQUIRE(!bits.get(2));
			REQUIRE(!bits.get(3));
			REQUIRE(!bits.get(4));
			REQUIRE(!bits.get(5));
			REQUIRE(!bits.get(6));
		}

		{
			Bits bits = bitArray.bits<3, 7>();
			bits.set<4>();
		}

		{
			const Bits bits = bitArray.bits<2, 13>();
			REQUIRE(!bits.get( 0));
			REQUIRE(!bits.get( 1));
			REQUIRE(!bits.get( 2));
			REQUIRE(!bits.get( 3));
			REQUIRE(!bits.get( 4));
			REQUIRE(!bits.get( 5));
			REQUIRE(!bits.get( 6));
			REQUIRE(!bits.get( 7));
			REQUIRE(!bits.get( 8));
			REQUIRE(!bits.get( 9));
			REQUIRE(!bits.get(10));
			REQUIRE(!bits.get(11));
			REQUIRE( bits.get(12));
		}

		{
			Bits bits = bitArray.bits<2, 13>();
			bits.clear<12>();
		}

		{
			const Bits bits = bitArray.bits<3, 7>();
			REQUIRE(!bits);

			REQUIRE(!bits.get(0));
			REQUIRE(!bits.get(1));
			REQUIRE(!bits.get(2));
			REQUIRE(!bits.get(3));
			REQUIRE(!bits.get(4));
			REQUIRE(!bits.get(5));
			REQUIRE(!bits.get(6));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

}
