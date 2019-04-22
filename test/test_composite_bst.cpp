#include "test_composite_bst.hpp"

using namespace test_composite_bst;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<S0>(),
		FSM::stateId<S1>(),
		FSM::stateId<S2>(),
		FSM::stateId<S3>(),
		FSM::stateId<S4>(),
		FSM::stateId<S5>(),
		FSM::stateId<S6>(),
		FSM::stateId<S7>(),
		FSM::stateId<S8>(),
		FSM::stateId<S9>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Composite BST", "[machine]") {
	Context _;
	Logger logger;

	FSM::Instance machine(_, &logger);

	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<S0>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::ENTER },
			{ FSM::stateId<S0>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<S0>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.changeTo<S1>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<S1>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<S0>(),	 Event::UPDATE },

			{ FSM::stateId<S0>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<S1>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<S0>(),	 Event::EXIT },
			{ FSM::stateId<S1>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<S1>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<S0>(),
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////
