#include "test_internal_transitions.hpp"

using namespace test_internal_transitions;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<A>(),
		FSM::stateId<A_1>(),
		FSM::stateId<A_2>(),
		FSM::stateId<A_2_1>(),
		FSM::stateId<A_2_2>(),
		FSM::stateId<B>(),
		FSM::stateId<B_1>(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_1_2>(),
		FSM::stateId<B_2>(),
		FSM::stateId<B_2_1>(),
		FSM::stateId<B_2_2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Internal transition", "[machine]") {
	Context _;
	Logger logger;
	FSM::Instance machine(_, &logger);

	{
		const Events reference = {
			{ FSM::stateId<A>(),	Event::ENTER },
			{ FSM::stateId<A_1>(),	Event::ENTER },
		};
		logger.assertSequence(reference);
	}

	{
		const Types states = {
			FSM::stateId<A>(),
			FSM::stateId<A_1>(),
		};
		assertActive(machine, all, states);
		assertResumable(machine, all, {});
	}
}

////////////////////////////////////////////////////////////////////////////////
