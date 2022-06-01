// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_PLANS
#include "../tools.hpp"

namespace test_task_list {

////////////////////////////////////////////////////////////////////////////////

using List = hfsm2::detail::TaskListT<void, 16>;

//------------------------------------------------------------------------------

TEST_CASE("Shared.List<>") {
	List list;
	constexpr auto CAPACITY = List::CAPACITY;

	REQUIRE(list.count()  == 0); //-V521

	WHEN("fill, delete and re-insert an element") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(static_cast<hfsm2::Long>(i),
											static_cast<hfsm2::Long>(i),
											hfsm2::TransitionType::COUNT);

			REQUIRE(index == i); //-V521
			REQUIRE(list.count() == i + 1); //-V521
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == hfsm2::detail::TaskBase{static_cast<hfsm2::Long>(i),
													   static_cast<hfsm2::Long>(i),
													   hfsm2::TransitionType::COUNT}); //-V521

		THEN("at the start") {
			REQUIRE(list.count() == CAPACITY); //-V521

			list.remove(0);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(static_cast<hfsm2::Long>(0u),
											static_cast<hfsm2::Long>(0u),
											hfsm2::TransitionType::COUNT);
			REQUIRE(index == 0); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}

		AND_THEN("at the mid") {
			REQUIRE(list.count() == CAPACITY); //-V521

			constexpr List::Index mid = CAPACITY / 2;
			list.remove(mid);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(static_cast<hfsm2::Long>(mid),
											static_cast<hfsm2::Long>(mid),
											hfsm2::TransitionType::COUNT);
			REQUIRE(index == mid); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}

		AND_THEN("at the end") {
			REQUIRE(list.count() == CAPACITY); //-V521

			constexpr List::Index end = CAPACITY - 1;
			list.remove(end);
			REQUIRE(list.count() == CAPACITY - 1); //-V521

			const auto index = list.emplace(static_cast<hfsm2::Long>(end),
											static_cast<hfsm2::Long>(end),
											hfsm2::TransitionType::COUNT);
			REQUIRE(index == end); //-V521
			REQUIRE(list.count() == CAPACITY); //-V521
		}
	}

	WHEN("fill, delete all and re-insert all elements") {
		for (List::Index i = 0; i < CAPACITY; ++i) {
			const auto index = list.emplace(static_cast<hfsm2::Long>(i),
											static_cast<hfsm2::Long>(i),
											hfsm2::TransitionType::COUNT);

			REQUIRE(index == i); //-V521
			REQUIRE(list.count() == i + 1); //-V521
		}

		for (List::Index i = 0; i < CAPACITY; ++i)
			REQUIRE(list[i] == hfsm2::detail::TaskBase{static_cast<hfsm2::Long>(i),
													   static_cast<hfsm2::Long>(i),
													   hfsm2::TransitionType::COUNT}); //-V521

		THEN("from the start") {
			REQUIRE(list.count() == CAPACITY); //-V521

			for (List::Index i = 0; i < CAPACITY; ++i) {
				list.remove(i);
				REQUIRE(list.count() == CAPACITY - 1 - i); //-V521
			}

			for (List::Index i = 0; i < CAPACITY; ++i) {
				const auto index = list.emplace(static_cast<hfsm2::Long>(i),
												static_cast<hfsm2::Long>(i),
												hfsm2::TransitionType::COUNT);

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
				const auto index = list.emplace(static_cast<hfsm2::Long>(i),
												static_cast<hfsm2::Long>(i),
												hfsm2::TransitionType::COUNT);

				REQUIRE(index == i); //-V521
				REQUIRE(list.count() == i + 1); //-V521
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

}
