#include "test_utility.hpp"

using namespace test_utility;

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

TEST_CASE("Utility test", "[machine]") {
	Context _;
	Logger logger;

	FSM::Instance machine(_, &logger);

	{
		const Events reference = {
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

	//machine.utilize<Apex>();
}

////////////////////////////////////////////////////////////////////////////////
