#include "../shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using List = hfsm2::detail::List<unsigned, 16>;

//------------------------------------------------------------------------------

TEST_CASE("List<> test", "[shared]") {
	List list;
	constexpr auto CAPACITY = List::CAPACITY;

	REQUIRE(list.count()  == 0); //-V521

	WHEN("fill, delete and re-insert an element") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(i);

			REQUIRE(index == i); //-V521
			REQUIRE(list.count() == i + 1); //-V521
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == i); //-V521

		THEN("at the start") {
			REQUIRE(list.count() == CAPACITY); //-V521

			list.remove(0);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(0u);
			REQUIRE(index == 0); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}

		AND_THEN("at the mid") {
			REQUIRE(list.count() == CAPACITY); //-V521

			constexpr List::Index mid = CAPACITY / 2;
			list.remove(mid);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(mid);
			REQUIRE(index == mid); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}

		AND_THEN("at the end") {
			REQUIRE(list.count() == CAPACITY); //-V521

			constexpr List::Index end = CAPACITY - 1;
			list.remove(end);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(end);
			REQUIRE(index == end); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}
	}

	WHEN("fill, delete all and re-insert all elements") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(i);

			REQUIRE(index == i); //-V521
			REQUIRE(list.count() == i + 1); //-V521
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == i); //-V521

		THEN("from the start") {
			REQUIRE(list.count() == CAPACITY); //-V521

			for (List::Index i = 0; i < CAPACITY; ++i) {
				list.remove(i);
				REQUIRE(list.count() == CAPACITY - 1 - i); //-V521
			}

			for (List::Index i = 0; i < CAPACITY; ++i) {
				const auto index = list.emplace(i);

				REQUIRE(index == CAPACITY - 1 - i); //-V521
				REQUIRE(list.count() == i + 1); //-V521
			}
		}

		AND_THEN("at the mid") {
			REQUIRE(list.count() == CAPACITY); //-V521
		}

		AND_THEN("from the end") {
			REQUIRE(list.count() == CAPACITY); //-V521

			for (List::Index i = 0; i < CAPACITY; ++i) {
				list.remove(CAPACITY - 1 - i);
				REQUIRE(list.count() == CAPACITY - 1 - i); //-V521
			}

			for (List::Index i = 0; i < CAPACITY; ++i) {
				const auto index = list.emplace(i);

				REQUIRE(index == i); //-V521
				REQUIRE(list.count() == i + 1); //-V521
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
