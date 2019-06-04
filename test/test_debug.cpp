#include "test_debug.hpp"

#include <algorithm>
#include <vector>

#include <string.h>

using namespace test_debug;

////////////////////////////////////////////////////////////////////////////////

using StructureReference = std::vector<hfsm2::StructureEntry>;

void
assertStructure(const FSM::Instance::Structure& structure,
				const StructureReference& reference)
{
	const auto count = std::max((size_t) structure.count(), reference.size());

	for (uint16_t i = 0; i < count; ++i) {
		REQUIRE(i < structure.count());
		REQUIRE(i < reference.size());

		if (i < structure.count() &&
			i < reference.size())
		{
			REQUIRE(structure[i].isActive == reference[i].isActive);
			REQUIRE(wcscmp(structure[i].prefix, reference[i].prefix) == 0);

		#ifdef _MSC_VER
			REQUIRE(strcmp(structure[i].name,   reference[i].name)	 == 0);
		#endif
		}
	}
}

//------------------------------------------------------------------------------

using ActivityReference = std::vector<char>;

void
assertActivity(const FSM::Instance::ActivityHistory& activity,
			   const ActivityReference& reference)
{
	const auto count = std::max((size_t) activity.count(), reference.size());

	for (uint16_t i = 0; i < count; ++i) {
		REQUIRE(i < activity.count());
		REQUIRE(i < reference.size());

		if (i < activity.count() &&
			i < reference.size())
		{
			REQUIRE(activity[i] == reference[i]);
		}
	}

}

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<I>(),
		FSM::stateId<O>(),
		FSM::stateId<R>(),
		FSM::stateId<R_1>(),
		FSM::stateId<R_2>(),
		FSM::stateId<C>(),
		FSM::stateId<C_1>(),
		FSM::stateId<C_2>(),
		FSM::stateId<U>(),
		FSM::stateId<U_1>(),
		FSM::stateId<U_2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Debug", "[machine]") {
	Logger logger;

	FSM::Instance machine(&logger);
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<I>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::ENTER },
			{ FSM::stateId<I>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<I>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});

		assertStructure(machine.structure(), {
			hfsm2::StructureEntry{ true,  L"", "Apex"},
			hfsm2::StructureEntry{ true,  L" ├ ", "I"},
			hfsm2::StructureEntry{ false, L" └ ", "O"},
			hfsm2::StructureEntry{ false, L"   ╟ ", "R"},
			hfsm2::StructureEntry{ false, L"   ║ ├ ", "R_1"},
			hfsm2::StructureEntry{ false, L"   ║ └ ", "R_2"},
			hfsm2::StructureEntry{ false, L"   ╟ ", "C"},
			hfsm2::StructureEntry{ false, L"   ║ ├ ", "C_1"},
			hfsm2::StructureEntry{ false, L"   ║ └ ", "C_2"},
			hfsm2::StructureEntry{ false, L"   ╙ ", "U"},
			hfsm2::StructureEntry{ false, L"     ├ ", "U_1"},
			hfsm2::StructureEntry{ false, L"     └ ", "U_2"},
		});

		assertActivity(machine.activityHistory(), {
			 1,
			 1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
		});
	}

	machine.changeTo<O>();
	machine.update();
	{
		const Events reference = {
			{						Event::CHANGE, FSM::stateId<O>() },

			{ FSM::stateId<Apex>(),	Event::UPDATE },
			{ FSM::stateId<I>(),	Event::UPDATE },

			{ FSM::stateId<U_1>(),	Event::UTILITY },
			{ FSM::stateId<U_2>(),	Event::UTILITY },
			{ FSM::stateId<U>(),	Event::UTILITY_RESOLUTION, 0 },

			{ FSM::stateId<I>(),	Event::EXIT_GUARD },

			{ FSM::stateId<O>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<R>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<R_1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<C>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<C_1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<U>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<U_1>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<I>(),	Event::EXIT },

			{ FSM::stateId<O>(),	Event::ENTER },
			{ FSM::stateId<R>(),	Event::ENTER },
			{ FSM::stateId<R_1>(),	Event::ENTER },
			{ FSM::stateId<C>(),	Event::ENTER },
			{ FSM::stateId<C_1>(),	Event::ENTER },
			{ FSM::stateId<U>(),	Event::ENTER },
			{ FSM::stateId<U_1>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<O>(),
			FSM::stateId<R>(),
			FSM::stateId<R_1>(),
			FSM::stateId<C>(),
			FSM::stateId<C_1>(),
			FSM::stateId<U>(),
			FSM::stateId<U_1>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<I>(),
		};
		assertResumable(machine, all, resumable);

		assertStructure(machine.structure(), {
			hfsm2::StructureEntry{ true,  L"", "Apex"},
			hfsm2::StructureEntry{ false, L" ├ ", "I"},
			hfsm2::StructureEntry{ true,  L" └ ", "O"},
			hfsm2::StructureEntry{ true,  L"   ╟ ", "R"},
			hfsm2::StructureEntry{ true,  L"   ║ ├ ", "R_1"},
			hfsm2::StructureEntry{ false, L"   ║ └ ", "R_2"},
			hfsm2::StructureEntry{ true,  L"   ╟ ", "C"},
			hfsm2::StructureEntry{ true,  L"   ║ ├ ", "C_1"},
			hfsm2::StructureEntry{ false, L"   ║ └ ", "C_2"},
			hfsm2::StructureEntry{ true,  L"   ╙ ", "U"},
			hfsm2::StructureEntry{ true,  L"     ├ ", "U_1"},
			hfsm2::StructureEntry{ false, L"     └ ", "U_2"},
		});

		assertActivity(machine.activityHistory(), {
			 2,
			-1,
			 1,
			 1,
			 1,
			-2,
			 1,
			 1,
			-2,
			 1,
			 1,
			-2,
		});
	}
}

////////////////////////////////////////////////////////////////////////////////
