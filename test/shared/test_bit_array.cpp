#include "../shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using BitArray = hfsm2::detail::BitArray<hfsm2::ShortIndex, 6>;
using Bits		= typename BitArray::Bits;

TEST_CASE("Shared.BitArray<>", "[shared]") {
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
			bits.reset<12>();
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
			bits.reset<12>();
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
