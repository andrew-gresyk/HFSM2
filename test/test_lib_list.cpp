#include "test_shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using List = hfsm::detail::List<unsigned, 16>;

//------------------------------------------------------------------------------

TEST_CASE("list<> test", "[lib]") {
	List list;
	constexpr auto CAPACITY = List::CAPACITY;

	REQUIRE(list.count()  == 0);

	WHEN("fill, delete and re-insert an element") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(i);

			REQUIRE(index == i);
			REQUIRE(list.count() == i + 1);
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == i);

		THEN("at the start") {
			REQUIRE(list.count() == CAPACITY);

			list.remove(0);
			REQUIRE(list.count() == CAPACITY - 1);

			const auto index = list.emplace(0u);
			REQUIRE(index == 0);
			REQUIRE(list.count() == CAPACITY);
		}

		AND_THEN("at the mid") {
			REQUIRE(list.count() == CAPACITY);

			constexpr List::Index mid = CAPACITY / 2;
			list.remove(mid);
			REQUIRE(list.count() == CAPACITY - 1);

			const auto index = list.emplace(mid);
			REQUIRE(index == mid);
			REQUIRE(list.count() == CAPACITY);
		}

		AND_THEN("at the end") {
			REQUIRE(list.count() == CAPACITY);

			constexpr List::Index end = CAPACITY - 1;
			list.remove(end);
			REQUIRE(list.count() == CAPACITY - 1);

			const auto index = list.emplace(end);
			REQUIRE(index == end);
			REQUIRE(list.count() == CAPACITY);
		}
	}

	WHEN("fill, delete all and re-insert all elements") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(i);

			REQUIRE(index == i);
			REQUIRE(list.count() == i + 1);
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == i);

		THEN("from the start") {
			REQUIRE(list.count() == CAPACITY);

			for (List::Index i = 0; i < CAPACITY; ++i) {
				list.remove(i);
				REQUIRE(list.count() == CAPACITY - 1 - i);
			}

			for (List::Index i = 0; i < CAPACITY; ++i) {
				const auto index = list.emplace(i);

				REQUIRE(index == CAPACITY - 1 - i);
				REQUIRE(list.count() == i + 1);
			}
		}

		AND_THEN("at the mid") {
			REQUIRE(list.count() == CAPACITY);
		}

		AND_THEN("from the end") {
			REQUIRE(list.count() == CAPACITY);

			for (List::Index i = 0; i < CAPACITY; ++i) {
				list.remove(CAPACITY - 1 - i);
				REQUIRE(list.count() == CAPACITY - 1 - i);
			}

			for (List::Index i = 0; i < CAPACITY; ++i) {
				const auto index = list.emplace(i);

				REQUIRE(index == i);
				REQUIRE(list.count() == i + 1);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
