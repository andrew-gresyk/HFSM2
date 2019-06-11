#include "test_ortho_units.hpp"

using namespace test_ortho_units;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<O1>(),
		FSM::stateId<O1_01>(),
		FSM::stateId<O1_02>(),
		FSM::stateId<O1_03>(),
		FSM::stateId<O1_04>(),
		FSM::stateId<O1_05>(),

		FSM::stateId<O2>(),
		FSM::stateId<O2_01>(),
		FSM::stateId<O2_02>(),
		FSM::stateId<O2_03>(),
		FSM::stateId<O2_04>(),
		FSM::stateId<O2_05>(),

		FSM::stateId<O3>(),
		FSM::stateId<O3_01>(),
		FSM::stateId<O3_02>(),
		FSM::stateId<O3_03>(),
		FSM::stateId<O3_04>(),
		FSM::stateId<O3_05>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.OrthoUnits", "[machine]") {
	Logger logger;

	FSM::Instance machine(&logger);
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<O1>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<O1_01>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_02>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_03>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_04>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_05>(), Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	 Event::ENTER },
			{ FSM::stateId<O1>(),	 Event::ENTER },
			{ FSM::stateId<O1_01>(), Event::ENTER },
			{ FSM::stateId<O1_02>(), Event::ENTER },
			{ FSM::stateId<O1_03>(), Event::ENTER },
			{ FSM::stateId<O1_04>(), Event::ENTER },
			{ FSM::stateId<O1_05>(), Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<O1>(),
			FSM::stateId<O1_01>(),
			FSM::stateId<O1_02>(),
			FSM::stateId<O1_03>(),
			FSM::stateId<O1_04>(),
			FSM::stateId<O1_05>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}
}

////////////////////////////////////////////////////////////////////////////////
